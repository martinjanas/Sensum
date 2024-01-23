#include "features.h"
#include "../settings/globals.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"
#include "../helpers/console.h"
#include "../hooks/hooks.h"
#include "../render/render.h"
#include "../helpers/autowall.h"
#include "../helpers/entities.h"
#include "../valve_sdk/classids.h"
#include "../helpers/notifies.h"

#include <mutex>
#include <algorithm>

namespace visuals
{
	std::mutex render_mutex;
	decltype(entities::m_local) m_local;

	int x, y;
	int xx, yy;

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

		std::string name;

		auto grenade = reinterpret_cast<c_base_combat_weapon*>(entity);

		if (!grenade)
			return;

		if (class_id == classids::CBaseCSGrenadeProjectile || class_id == classids::CMolotovProjectile || class_id == classids::CDecoyProjectile 
			|| class_id == classids::CSmokeGrenadeProjectile || class_id == classids::CSensorGrenadeProjectile) 
		{
			if (entity->m_hOwnerEntity())
				name = entity->m_hOwnerEntity().Get()->GetPlayerInfo().szName;
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

	void rcs_cross(ImDrawList* draw_list, entities::local_data_t& local)
	{
		if (!g::local_player)
			return;

		if (!local.local)
			return;

		if (local.local->GetIndex() != g::local_player->GetIndex())
			return;

		if (!local.is_alive)
			return;

		if (local.shots_fired <= 1 && local.punch_angle.pitch >= -0.46f)
			return;
		
		static auto weapon_recoil_scale = g::cvar->find("weapon_recoil_scale");
		float scale = weapon_recoil_scale->GetFloat();

		QAngle viewangles;
		g::engine_client->GetViewAngles(viewangles);

		viewangles += local.punch_angle * scale;

		Vector forward;
		math::angle2vectors(viewangles, forward);
		forward *= 10000;

		Vector start = local.eye_pos;
		Vector end = start + forward, out;

		if (!math::world2screen(end, out))
			return;
		
		switch (settings::visuals::rcs_cross_mode)
		{
		case 0:
			draw_list->AddRectFilled(ImVec2{ out.x - 11, out.y - 1 }, ImVec2{ out.x - 4.f, out.y + 1 }, ImGui::GetColorU32(settings::visuals::recoil_color));
 			draw_list->AddRectFilled(ImVec2{ out.x - 10, out.y }, ImVec2{ out.x - 5.f, out.y + 0.5f }, ImGui::GetColorU32(settings::visuals::recoil_color));
 
 			// right
 			draw_list->AddRectFilled(ImVec2{ out.x + 4, out.y - 1 }, ImVec2{ out.x + 10.5f, out.y + 1 }, ImGui::GetColorU32(settings::visuals::recoil_color));
 			draw_list->AddRectFilled(ImVec2{ out.x + 5, out.y }, ImVec2{ out.x + 9.5f, out.y + 0.5f }, ImGui::GetColorU32(settings::visuals::recoil_color));
 
 			// top (left with swapped x/y offsets)
 			draw_list->AddRectFilled(ImVec2{ out.x - 1, out.y - 11 }, ImVec2{ out.x + 1.f, out.y - 4.f }, ImGui::GetColorU32(settings::visuals::recoil_color));
 			draw_list->AddRectFilled(ImVec2{ out.x, out.y - 10 }, ImVec2{ out.x, out.y - 5.f }, ImGui::GetColorU32(settings::visuals::recoil_color));
 
 			// bottom (right with swapped x/y offsets)
 			draw_list->AddRectFilled(ImVec2{ out.x - 1, out.y + 4 }, ImVec2{ out.x + 1.f, out.y + 10.5f }, ImGui::GetColorU32(settings::visuals::recoil_color));
 			draw_list->AddRectFilled(ImVec2{ out.x, out.y + 5 }, ImVec2{ out.x, out.y + 9.5f }, ImGui::GetColorU32(settings::visuals::recoil_color));
			break;
		case 1:
			draw_list->AddCircle(ImVec2(out.x, out.y), settings::visuals::radius, ImGui::GetColorU32(settings::visuals::recoil_color), 255);
			break;
		}
	}

	void noscope(ImDrawList* draw_list, entities::local_data_t& local, int xx, int yy)
	{
		if (!g::local_player)
			return;

		if (!local.local)
			return;

		if (local.local->GetIndex() != g::local_player->GetIndex())
			return;

		if (!local.is_alive)
			return;

		auto& active_wpn = local.active_weapon;

		if (!active_wpn)
			return;

		static const ImVec4 black = ImVec4(0.f, 0.f, 0.f, 1.0f);

		if (local.is_scoped && active_wpn->IsSniper())
		{
			draw_list->AddLine(ImVec2(0, yy), ImVec2((xx * 2), yy), ImGui::GetColorU32(black));
			draw_list->AddLine(ImVec2(xx, 0), ImVec2(xx, (yy * 2)), ImGui::GetColorU32(black));
			draw_list->AddCircle(ImVec2(xx, yy), 255, ImGui::GetColorU32(black), 255);
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

		const auto highest_entity_index = g::entity_list->GetHighestEntityIndex();
		for (auto i = 1; i <= highest_entity_index; ++i)
		{
			auto* entity = c_base_player::GetPlayerByIndex(i);

			if (!entity || entity->IsPlayer() || entity->IsDormant() || entity == g::local_player)
				continue;

			auto* bomb = reinterpret_cast<c_base_planted_c4*>(entity);

			float dist = g::local_player->m_vecOrigin().DistTo(entity->m_vecOrigin());
	
			const auto classid = entity->GetClientClass()->m_ClassID;
			if (settings::visuals::world_grenades && (classid == classids::CBaseCSGrenadeProjectile || classid == classids::CMolotovProjectile || classid == classids::CSmokeGrenadeProjectile || classid == classids::CDecoyProjectile))
				world_grenades(entity);
			else if (settings::visuals::planted_c4 && entity->IsPlantedC4() && bomb->m_bBombTicking())
				push_entity(entity, "Bomb", "", false, Color::Green);
			else if (settings::visuals::dropped_weapons && entity->IsWeapon() && !entity->m_hOwnerEntity().IsValid() && dist <= 1000.f)
				push_entity(entity, utils::get_weapon_name(entity), "", false, Color::White);
		}

		render_mutex.unlock();
	}

	void render(ImDrawList* draw_list)
	{
		if (!is_enabled() || !render::fonts::visuals || g::engine_client->IsConsoleVisible())
			return;
		
		if (entities::local_mutex.try_lock())
		{
			m_local = entities::m_local;
			entities::local_mutex.unlock();
		}

		if (render_mutex.try_lock())
		{
			saved_entities = entities;
			render_mutex.unlock();
		}

		g::engine_client->GetScreenSize(x, y);

		xx = x / 2;
		yy = y / 2;

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
		
		if (settings::visuals::rcs_cross)
			rcs_cross(draw_list, m_local);

		if (settings::misc::noscope)
			noscope(draw_list, m_local, xx, yy);
	}
}