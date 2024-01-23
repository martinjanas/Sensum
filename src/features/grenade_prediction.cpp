#include "features.h"
#include "../settings/options.hpp"
#include "../render/render.h"
#include "../helpers/math.h"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../helpers/imdraw.h"

#include <mutex>

enum ACT
{
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP,
};

namespace grenade_prediction
{
	int act = 0;
	bool has_grenade = false;
	bool was_flashed = false;

	std::vector<std::pair<Vector, Vector>> screen_points;
	std::vector<std::pair<Vector, Vector>> saved_points;

	c_base_player* player = nullptr;
	c_base_combat_weapon* weapon = nullptr;

	std::mutex render_mutex;

	void Tick(int buttons)
	{
		if (!settings::visuals::grenade_prediction)
			return;

		bool in_attack = buttons & IN_ATTACK;
		bool in_attack2 = buttons & IN_ATTACK2;

		act = (in_attack && in_attack2) ? ACT_LOB :
			(in_attack2) ? ACT_DROP :
			(in_attack) ? ACT_THROW :
			ACT_NONE;
	}

	bool is_enabled(CUserCmd* cmd)
	{
		if (!g::local_player)
			return false;

		player = g::local_player;
		if (player->IsAlive())
			Tick(cmd->buttons);
		else if (player->m_hObserverTarget())
		{
			player = (c_base_player*)c_base_player::GetEntityFromHandle(player->m_hObserverTarget());
			act = 0;
		}
		else
			return false;

		if (player->m_nMoveType() == MOVETYPE_NOCLIP)
			return false;

		weapon = player->m_hActiveWeapon();
		if (!weapon || !weapon->IsGrenade())
			return false;

		return true;
	}

	void fetch_points(CUserCmd* cmd)
	{
		render_mutex.lock();

		screen_points.clear();

		if (!is_enabled(cmd))
		{
			render_mutex.unlock();
			return;
		}

		was_flashed = player->IsFlashed();

		Vector vecSrc, vecThrow;
		Setup(vecSrc, vecThrow, cmd->viewangles);

		float interval = g::global_vars->interval_per_tick;
		int logstep = static_cast<int>(0.05f / interval);
		int logtimer = 0;

		std::vector<Vector> path;

		for (unsigned int i = 0; i < path.max_size() - 1; ++i)
		{
			if (!logtimer)
				path.emplace_back(vecSrc);

			Vector move;
			AddGravityMove(move, vecThrow, interval, false);

			// Push entity
			trace_t tr;
			PushEntity(vecSrc, move, tr);

			int result = 0;
			if (weapon->check_detonate(vecThrow, tr, i, interval))
				result |= 1;

			if (tr.fraction != 1.0f)
			{
				result |= 2; // Collision!
				ResolveFlyCollisionCustom(tr, vecThrow, interval);
			}

			vecSrc = tr.endpos;

			if (result & 1)
				break;

			if ((result & 2) || logtimer >= logstep)
				logtimer = 0;
			else
				++logtimer;
		}

		path.emplace_back(vecSrc);

		Vector prev = path[0];
		Vector nadeStart, nadeEnd;

		for (auto& nade : path)
		{
			if (math::world2screen(prev, nadeStart) && math::world2screen(nade, nadeEnd))
			{
				screen_points.emplace_back(std::pair<Vector, Vector>{ nadeStart, nadeEnd });
				prev = nade;
			}
		}

		render_mutex.unlock();
	}

	void Setup(Vector& vecSrc, Vector& vecThrow, QAngle viewangles)
	{
		QAngle angThrow = viewangles;
		float pitch = angThrow.pitch;

		if (pitch <= 90.0f)
		{
			if (pitch < -90.0f)
			{
				pitch += 360.0f;
			}
		}
		else
		{
			pitch -= 360.0f;
		}

		float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
		angThrow.pitch = a;

		float flVel = 750.0f * 0.9f;

		static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
		float b = power[act];
		b = b * 0.7f;
		b = b + 0.3f;
		flVel *= b;

		Vector vForward, vRight, vUp;
		math::angle2vectors(angThrow, &vForward, &vRight, &vUp);

		vecSrc = g::local_player->GetEyePos();
		float off = (power[act] * 12.0f) - 12.0f;
		vecSrc.z += off;

		trace_t tr;
		Vector vecDest = vecSrc;
		vecDest += vForward * 22.0f;

		TraceHull(vecSrc, vecDest, tr);

		Vector vecBack = vForward; vecBack *= 6.0f;
		vecSrc = tr.endpos;
		vecSrc -= vecBack;

		vecThrow = g::local_player->m_vecVelocity(); vecThrow *= 1.25f;
		vecThrow += vForward * flVel;
	}

	void ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
	{
		if (!settings::visuals::grenade_prediction)
			return;

		// Calculate elasticity
		float flSurfaceElasticity = 1.0;
		float flGrenadeElasticity = 0.45f;
		float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
		if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
		if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

		// Calculate bounce
		Vector vecAbsVelocity;
		PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
		vecAbsVelocity *= flTotalElasticity;

		float flSpeedSqr = vecAbsVelocity.LengthSqr();
		static const float flMinSpeedSqr = 20.0f * 20.0f;

		if (flSpeedSqr < flMinSpeedSqr)
		{
			vecAbsVelocity.x = 0.0f;
			vecAbsVelocity.y = 0.0f;
			vecAbsVelocity.z = 0.0f;
		}

		if (tr.plane.normal.z > 0.7f)
		{
			vecVelocity = vecAbsVelocity;
			vecAbsVelocity *= ((1.0f - tr.fraction) * interval);
			PushEntity(tr.endpos, vecAbsVelocity, tr);
		}
		else
		{
			vecVelocity = vecAbsVelocity;
		}
	}

	void TraceHull(Vector& src, Vector& end, trace_t& tr)
	{
		if (!settings::visuals::grenade_prediction)
			return;

		Ray_t ray;
		ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

		CTraceFilterWorldAndPropsOnly filter;

		g::engine_trace->trace_ray(ray, 0x200400B, &filter, &tr);
	}

	void PushEntity(Vector& src, const Vector& move, trace_t& tr)
	{
		if (!settings::visuals::grenade_prediction)
			return;

		Vector vecAbsEnd = src;
		vecAbsEnd += move;
		TraceHull(src, vecAbsEnd, tr);
	}

	void AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
	{
		if (!settings::visuals::grenade_prediction)
			return;

		Vector basevel(0.0f, 0.0f, 0.0f);

		move.x = (vel.x + basevel.x) * frametime;
		move.y = (vel.y + basevel.y) * frametime;

		if (onground)
		{
			move.z = (vel.z + basevel.z) * frametime;
		}
		else
		{
			float gravity = 800.0f * 0.4f;
			float newZ = vel.z - (gravity * frametime);
			move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;
			vel.z = newZ;
		}
	}

	int PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
	{
		static const float STOP_EPSILON = 0.1f;

		float    backoff;
		float    change;
		float    angle;
		int        i, blocked;

		blocked = 0;

		angle = normal[2];

		if (angle > 0)
		{
			blocked |= 1;        // floor
		}
		if (!angle)
		{
			blocked |= 2;        // step
		}

		backoff = in.Dot(normal) * overbounce;

		for (i = 0; i < 3; i++)
		{
			change = normal[i] * backoff;
			out[i] = in[i] - change;
			if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			{
				out[i] = 0;
			}
		}

		return blocked;
	}

	void render(ImDrawList* draw_list)
	{
		if (render::menu::is_visible() || !g::engine_client->IsInGame() || !g::engine_client->IsConnected() || g::engine_client->IsConsoleVisible())
			return;

		if (render_mutex.try_lock())
		{
			saved_points = screen_points;

			render_mutex.unlock();
		}

		if (saved_points.empty())
			return;

		static const auto red_color = ImGui::GetColorU32(ImVec4::Red);
		static const auto white_color = ImGui::GetColorU32(ImVec4::White);
		static const auto black_color = ImGui::GetColorU32(ImVec4::Black);
		static const auto blue_color = ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.8f, 1.f));

		for (auto& point : saved_points)
		{
			draw_list->AddLine(ImVec2(point.first.x, point.first.y), ImVec2(point.second.x, point.second.y), was_flashed ? black_color : white_color, 1.5f);
		}

		const auto last_point = saved_points[saved_points.size() - 1].second;
		draw_list->AddCircle(ImVec2(last_point.x, last_point.y), 5.f, red_color);

		const auto& first_point = saved_points.at(0);
		draw_list->AddRectFilled(ImVec2(first_point.first.x - 5.f, first_point.first.y - 5.f), ImVec2(first_point.second.x + 5.f, first_point.second.y + 5.f), was_flashed ? black_color : white_color);
	}
}