#include "features.h"
#include "../settings/globals.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"
#include "../helpers/console.h"
#include "../features/esp.hpp"
#include "../hooks/hooks.h"
#include "../helpers/runtime_saver.h"
#include "../render/render.h"
#include "../helpers/autowall.h"
#include "../helpers/entities.h"

#include <mutex>

namespace visuals
{
	std::mutex render_mutex;

	decltype(entities::m_local) m_local;

	int x, y;

	struct entity_data_t
	{
		std::string text;
		std::string text2;
		Vector origin;
		Color color;
		bool is_grenade;
	};

	struct grenade_info_t
	{
		std::string name;
		Color color;
	};

	RECT GetBBox(c_base_entity* ent)
	{
		RECT rect{};
		auto collideable = ent->GetCollideable();

		if (!collideable)
			return rect;

		const auto& min = collideable->OBBMins();
		const auto& max = collideable->OBBMaxs();

		const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

		Vector points[] =
		{
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		Vector pointsTransformed[8];

		for (int i = 0; i < 8; i++)
			math::VectorTransform(points[i], trans, pointsTransformed[i]);

		Vector screen_points[8] = {};

		for (int i = 0; i < 8; i++)
		{
			if (!math::world2screen(pointsTransformed[i], screen_points[i]))
				return rect;
		}

		auto left = screen_points[0].x;
		auto top = screen_points[0].y;
		auto right = screen_points[0].x;
		auto bottom = screen_points[0].y;

		for (int i = 1; i < 8; i++)
		{
			if (left > screen_points[i].x)
				left = screen_points[i].x;

			if (top < screen_points[i].y)
				top = screen_points[i].y;

			if (right < screen_points[i].x)
				right = screen_points[i].x;

			if (bottom > screen_points[i].y)
				bottom = screen_points[i].y;
		}

		return RECT{ (long)left, (long)top, (long)right, (long)bottom };
	}

	std::vector<entity_data_t> entities;
	std::vector<entity_data_t> saved_entities;

	bool is_enabled()
	{
		return g::engine_client->IsConnected() && g::local_player && !render::menu::is_visible();
	}

	void push_entity(c_base_entity* entity, const std::string& text, const std::string& text2, bool is_grenade, const Color& color = Color::White)
	{
		entities.emplace_back(entity_data_t{ text, text2, entity->m_vecOrigin(), color, is_grenade });
	}

	void world_grenades(c_base_player* entity)
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return;

		if (g::local_player->IsFlashed())
			return;

		if (utils::is_line_goes_through_smoke(g::local_player->GetEyePos(), entity->GetRenderOrigin()))
			return;

		auto bbox = GetBBox(entity);
		auto class_id = entity->GetClientClass()->m_ClassID;

		std::string name = "";

		auto grenade = reinterpret_cast<c_base_combat_weapon*>(entity);

		if (!grenade)
			return;

		switch (class_id) {
			case EClassId::CBaseCSGrenadeProjectile:
			case EClassId::CMolotovProjectile:
			case EClassId::CDecoyProjectile:
			case EClassId::CSmokeGrenadeProjectile:
			case EClassId::CSensorGrenadeProjectile:
				if(entity->m_hOwnerEntity())
				   name = entity->m_hOwnerEntity().Get()->GetPlayerInfo().szName; break;
		}

		grenade_info_t info;
		const auto model_name = fnv::hash_runtime(g::mdl_info->GetModelName(entity->GetModel()));
		if (model_name == FNV("models/Weapons/w_eq_smokegrenade_thrown.mdl"))
			info = { "Smoke", Color::White };
		else if (model_name == FNV("models/Weapons/w_eq_flashbang_dropped.mdl"))
			info = { "Flash", Color::Yellow };
		else if (model_name == FNV("models/Weapons/w_eq_fraggrenade_dropped.mdl"))
			info = { "Grenade", Color::Red };
		else if (model_name == FNV("models/Weapons/w_eq_molotov_dropped.mdl") || model_name == FNV("models/Weapons/w_eq_incendiarygrenade_dropped.mdl"))
			info = { "Molly", Color::Orange };
		else if (model_name == FNV("models/Weapons/w_eq_decoy_dropped.mdl"))
			info = { "Decoy", Color::Green };

		if (!info.name.empty() && (grenade->m_nExplodeEffectTickBegin() < 1))
			push_entity(entity, info.name, name, true, info.color);
	}

	void leftknife()
	{
		static auto left_knife = g::cvar->find("cl_righthand");

		if (!g::local_player || !g::local_player->IsAlive())
		{
			left_knife->SetValue(1);
			return;
		}

		auto& weapon = g::local_player->m_hActiveWeapon();

		if (!weapon) 
			return;

		left_knife->SetValue(!weapon->IsKnife());
	}

	void draw_fov(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (settings::esp::drawFov)
		{
			if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
				return;

			if (local.local != g::local_player)
				return;

			if (!local.local)
				return;

			auto& pWeapon = local.active_weapon;

			if (!pWeapon)
				return;

			const auto& settings = settings::aimbot::m_items[pWeapon->m_iItemDefinitionIndex()];

			if (settings.enabled)
			{
				if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
					return;

				if (!local.is_alive)
					return;

				if (settings.dynamic_fov)
					return;

				if (settings.fov <= 0.f)
					return;

				Vector2D screenSize = Vector2D(x, y);
				Vector2D center = screenSize * 0.5f;

				float ratio = screenSize.x / screenSize.y;
				float screenFov = atanf((ratio) * (0.75f) * tan(DEG2RAD(static_cast<float>(g::local_player->GetFOV()) * 0.5f)));

				float radius = tanf(DEG2RAD(aimbot::get_fov())) / tanf(screenFov) * center.x;

				draw_list->AddCircle(ImVec2(center.x, center.y), radius, ImGui::GetColorU32(settings::visuals::drawfov_color), 255);
			}
		}
	}

	void rcs_cross(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (settings::visuals::rcs_cross)
		{
			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (!local.is_alive)
				return;

			int _x = xx;
			int _y = yy;
			int dy = yy / 97;
			int dx = xx / 97;

			_x -= (dx * (local.punch_angle.yaw));
			_y += (dy * (local.punch_angle.pitch));

			auto& active_wpn = local.local->m_hActiveWeapon();//local.active_weapon;

			if (!active_wpn)
				return;

			static auto index = active_wpn->m_iItemDefinitionIndex();

			if (index == WEAPON_USP_SILENCER || index == WEAPON_DEAGLE || index == WEAPON_GLOCK || index == WEAPON_REVOLVER || index == WEAPON_HKP2000)
				return;

			switch (settings::visuals::rcs_cross_mode)
			{
			case 0:
				if (local.shots_fired > 1)
				{
					draw_list->AddLine(ImVec2(_x - 5, _y), ImVec2(_x + 5, _y), ImGui::GetColorU32(settings::visuals::recoilcolor));
					draw_list->AddLine(ImVec2(_x, _y - 5), ImVec2(_x, _y + 5), ImGui::GetColorU32(settings::visuals::recoilcolor));
				}
				break;
			case 1:
				if (local.shots_fired > 1)
				{
					draw_list->AddCircle(ImVec2(_x, _y), settings::visuals::radius, ImGui::GetColorU32(settings::visuals::recoilcolor), 255);
				}
				break;
			}
		}
	}

	void hitmarker(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (settings::visuals::hitmarker)
		{
			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (local.local != g::local_player)
				return;

			if (!local.local)
				return;

			if (g::global_vars->realtime - saver.HitmarkerInfo.HitTime > .5f)
				return;

			float percent = (g::global_vars->realtime - saver.HitmarkerInfo.HitTime) / .5f;
			float percent2 = percent;

			if (percent > 1.f)
			{
				percent = 1.f;
				percent2 = 1.f;
			}

			percent = 1.f - percent;
			float addsize = percent2 * 5.f;

			ImVec4 clr = ImVec4{ 1.0f, 1.0f, 1.0f, percent * 1.0f };

			draw_list->AddLine(ImVec2(xx - 3.f - addsize, yy - 3.f - addsize), ImVec2(xx + 3.f + addsize, yy + 3.f + addsize), ImGui::GetColorU32(clr));
			draw_list->AddLine(ImVec2(xx - 3.f - addsize, yy + 3.f + addsize), ImVec2(xx + 3.f + addsize, yy - 3.f - addsize), ImGui::GetColorU32(clr));
		}
	}

	void noscope(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (settings::misc::noscope)
		{
			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (local.local != g::local_player)
				return;

			if (!local.local)
				return;

			auto& active_wpn = local.active_weapon;

			if (!active_wpn)
				return;

			if (g::local_player->m_bIsScoped() && active_wpn->IsSniper())
			{
				draw_list->AddLine(ImVec2(0, yy), ImVec2(x, yy), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
				draw_list->AddLine(ImVec2(xx, 0), ImVec2(xx, y), ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }));
				draw_list->AddCircle(ImVec2(xx, yy), 255, ImGui::GetColorU32(ImVec4{ 0.f, 0.f, 0.f, 1.0f }), 255);
			}
		}
	}

	void spread_cross(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (settings::visuals::spread_cross)
		{
			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (local.local != g::local_player)
				return;

			if (!local.local)
				return;

			if (!local.is_alive)
				return;

			auto& active_wpn = local.active_weapon;

			if (!active_wpn)
				return;

			float spread = active_wpn->GetInaccuracy() * 1000.f;

			if (spread == 0.f)
				return;

			draw_list->AddCircle(ImVec2(xx, yy), spread, ImGui::GetColorU32(settings::visuals::spread_cross_color), 255);
			draw_list->AddCircleFilled(ImVec2(xx, yy), spread - 1, ImGui::GetColorU32({ 0.0f, 0.0f, 0.0f, 0.2f }), 255);
		}
	}

	void damage_indicator(entities::local_data_t& local)
	{
		if (settings::misc::damage_indicator)
		{
			if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
				return;

			if (local.local != g::local_player)
				return;

			if (!local.local && !local.is_alive)
				return;

			float CurrentTime = local.tick_base * g::global_vars->interval_per_tick;

			for (int i = 0; i < indicator.size(); i++)
			{
				if (indicator[i].flEraseTime < CurrentTime)
				{
					indicator.erase(indicator.begin() + i);
					continue;
				}

				if (!indicator[i].bInitialized)
				{
					indicator[i].Position = indicator[i].Player->get_bone_position(8);
					indicator[i].bInitialized = true;
				}

				if (CurrentTime - indicator[i].flLastUpdate > 0.001f)
				{
					indicator[i].Position.z -= (0.5f * (CurrentTime - indicator[i].flEraseTime));
					indicator[i].flLastUpdate = CurrentTime;
				}

				if (!indicator[i].Player)
				{
					indicator.erase(indicator.begin() + i);
					continue;
				}

				Vector ScreenPosition;

				Color color = Color::White;

				if (indicator[i].iDamage >= 100)
					color = Color::Red;

				if (indicator[i].iDamage >= 50 && indicator[i].iDamage < 100)
					color = Color::Orange;

				if (indicator[i].iDamage < 50)
					color = Color::White;

				if (math::world2screen(indicator[i].Position, ScreenPosition))
				{
					ImGui::PushFont(render::fonts::visuals);
					imdraw::outlined_text(std::to_string(indicator[i].iDamage).c_str(), ImVec2(ScreenPosition.x, ScreenPosition.y), utils::to_im32(color));
					ImGui::PopFont();
				}
			}
		}
	}

	void fetch_entities()
	{
		render_mutex.lock();

		entities.clear();

		if (!is_enabled())
		{
			render_mutex.unlock();
			return;
		}

		for (auto i = 1; i <= g::entity_list->GetHighestEntityIndex(); ++i)
		{
			auto* entity = c_base_player::GetPlayerByIndex(i);

			if (!entity || entity->IsPlayer() || entity->is_dormant() || entity == g::local_player)
				continue;

			const auto classid = entity->GetClientClass()->m_ClassID;
			if (settings::visuals::world_grenades && (classid == EClassId::CBaseCSGrenadeProjectile || classid == EClassId::CMolotovProjectile || classid == EClassId::CSmokeGrenadeProjectile || classid == EClassId::CDecoyProjectile))
				world_grenades(entity);
			else if (settings::visuals::planted_c4 && entity->IsPlantedC4())
				push_entity(entity, "Bomb", "", false, Color::Yellow);
			else if (settings::visuals::dropped_weapons && entity->IsWeapon() && !entity->m_hOwnerEntity().IsValid())
				push_entity(entity, utils::get_weapon_name(entity), "", false, Color::White);
		}

		render_mutex.unlock();
	}

	void render(ImDrawList* draw_list)
	{
		if (!is_enabled() || !render::fonts::visuals || g::engine_client->IsConsoleVisible())
			return;

		g::engine_client->GetScreenSize(x, y);

		static int xx = x / 2;
		static int yy = y / 2;

		if (render_mutex.try_lock())
		{
			saved_entities = entities;
			render_mutex.unlock();
		}

		if (entities::local_mutex.try_lock())
		{
			m_local = entities::m_local;
			entities::local_mutex.unlock();
		}

		ImGui::PushFont(render::fonts::visuals);
		{
			Vector origin;
			for (const auto& entity : saved_entities)
			{
				if (math::world2screen(entity.origin, origin))
				{
					const auto text_size = ImGui::CalcTextSize(entity.text.c_str());
					imdraw::outlined_text(entity.text.c_str(), ImVec2(origin.x - text_size.x / 2.f, origin.y + 7.f), utils::to_im32(entity.color));

					const auto text_size2 = ImGui::CalcTextSize(entity.text2.c_str());
					imdraw::outlined_text(entity.text2.c_str(), ImVec2(origin.x - text_size2.x / 2.f, origin.y + 15.f), utils::to_im32(Color::White));

					if (entity.is_grenade)
						draw_list->AddRect(ImVec2(origin.x + 7.f, origin.y + 7.f), ImVec2(origin.x - 7.f, origin.y - 7.f), utils::to_im32(entity.color));
				}
			}
		}
		ImGui::PopFont();

		damage_indicator(m_local);
		draw_fov(draw_list, m_local, xx, yy);
		noscope(draw_list, m_local, xx, yy);
		spread_cross(draw_list, m_local, xx, yy);
		hitmarker(draw_list, m_local, xx, yy);
		rcs_cross(draw_list, m_local, xx, yy);
	}
}