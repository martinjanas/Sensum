#include "features.h"
#include "../render/render.h"
#include "../sdk/classes/CPlantedC4.h"
#include "../sdk/helpers/Timer.h"
#include <format>

#include "../sdk/helpers/utils.h"
#include "../sdk/helpers/IconFetcher.h"

namespace features::esp
{
	std::list<entity_data::player_data_t> m_player_data;
	std::list<entity_data::entity_entry_data_t> m_entity_entry_data;

	void Draw3DBox(BBox_t& bbox)
	{
		const int BOTTOM_RIGHT_BACK = 0;
		const int BOTTOM_RIGHT_FRONT = 1;
		const int BOTTOM_LEFT_BACK = 2;
		const int BOTTOM_LEFT_FRONT = 3;
		const int TOP_RIGHT_BACK = 4;
		const int TOP_RIGHT_FRONT = 5;
		const int TOP_LEFT_BACK = 6;
		const int TOP_LEFT_FRONT = 7;

		/*
               6.........7
			   /		/|
		      /		   / |
			4┌--------┐5 |
			 | 	 ◯>   |  |
			 |	─┼─   |  |
		   2 |...│....|..|
			 ||	/ \   | / 3
			 ||       |/  
			0└--------┘1
				*/

		//Top face
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].as_vec2(), bbox.m_Vertices[TOP_LEFT_BACK].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_FRONT].as_vec2(), bbox.m_Vertices[TOP_RIGHT_BACK].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].as_vec2(), bbox.m_Vertices[TOP_RIGHT_FRONT].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_BACK].as_vec2(), bbox.m_Vertices[TOP_RIGHT_BACK].as_vec2(), IM_COL32_WHITE);

		//Bottom face
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_FRONT].as_vec2(), bbox.m_Vertices[BOTTOM_LEFT_BACK].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_RIGHT_FRONT].as_vec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_FRONT].as_vec2(), bbox.m_Vertices[BOTTOM_RIGHT_FRONT].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_BACK].as_vec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].as_vec2(), IM_COL32_WHITE);

		//Left face
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].as_vec2(), bbox.m_Vertices[BOTTOM_LEFT_FRONT].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_BACK].as_vec2(), bbox.m_Vertices[BOTTOM_LEFT_BACK].as_vec2(), IM_COL32_WHITE);

		//Right face
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_FRONT].as_vec2(), bbox.m_Vertices[BOTTOM_RIGHT_FRONT].as_vec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_BACK].as_vec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].as_vec2(), IM_COL32_WHITE);
	}

	void outlined_text(const char* text, const ImVec2& pos, const ImU32& color, const ImU32& outlined_color)
	{
		globals::draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) + 1.f), outlined_color, text);
		//globals::draw_list->AddText(ImVec2((pos.x) - 1.f, (pos.y) - 1.f), outlined_color, text);
		globals::draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) - 1.f), outlined_color, text);
		//globals::draw_list->AddText(ImVec2((pos.x) - 1.f, (pos.y) + 1.f), outlined_color, text);
		
		globals::draw_list->AddText(pos, color, text);
	}
	
	void outlined_text(const char* text, const ImVec2& pos, const ImU32& color)
	{
		auto black_color = imgui::GetColorU32(ImVec4(0, 0, 0, 1.f));
		outlined_text(text, pos, color, black_color);
	}
	
	//bomb_timer, grenade_projectiles and dropped_entities functions are in wip/prototype stage, they will be redone properly later
	void bomb_timer(const entity_data::bomb_info_t& bomb_info)
	{
		auto bomb_time = bomb_info.m_flC4Blow - g::global_vars->m_curtime;
		bomb_time = std::clamp<float>(bomb_time, 0.f, 40.f);
		if (bomb_time <= 0.0f)
			return;
		
		const auto& icon = bomb_info.bomb_icon;
		if (Vector pos; icon.texture && globals::world2screen(bomb_info.m_vecAbsOrigin, pos))
		{
			ImVec2 image_start = ImVec2(pos.x - icon.w / 2, pos.y - icon.h / 2 - 3.f);
			ImVec2 image_end = ImVec2(image_start.x + icon.w, image_start.y + icon.h);

			globals::draw_list->AddImage(icon.texture, image_start, image_end, ImVec2(0, 0), ImVec2(1, 1), imgui::ColorConvertFloat4ToU32(icon.tint));

			ImGui::PushFont(render::fonts::esp_small);
			outlined_text(std::format("{:.1f}", bomb_time).c_str(), {pos.as_vec2().x, pos.as_vec2().y + 25.f}, IM_COL32_WHITE);
			ImGui::PopFont();
		}
	}
	
	void grenade_projectiles(const entity_data::grenade_info_t& grenade_info)
	{
		const bool is_smoke = grenade_info.type == entity_data::GRENADE_SMOKE;
		const bool is_molotov = grenade_info.type == entity_data::GRENADE_MOLOTOV;
		const bool is_he = grenade_info.type == entity_data::GRENADE_HE;
		const bool is_flashbang = grenade_info.type == entity_data::GRENADE_FLASH;
		const bool is_decoy = grenade_info.type == entity_data::GRENADE_DECOY;
		
		const auto& icon = grenade_info.icon;
		if (Vector pos; icon.texture && globals::world2screen(grenade_info.m_vecOrigin, pos))
		{
			ImVec2 image_start = ImVec2(pos.x - icon.w / 2, pos.y - icon.h / 2 - 3.f);
			ImVec2 image_end = ImVec2(image_start.x + icon.w, image_start.y + icon.h);

			bool should_draw_image = true;
			if (is_he)
			{
				auto& he_info = std::get<entity_data::hegrenade_info_t>(grenade_info.specific_data);
				if (he_info.m_nExplodeEffectTickBegin >= 1)
					should_draw_image = false;
			}
			else if (is_smoke)
			{
				auto& smoke_info = std::get<entity_data::smoke_info_t>(grenade_info.specific_data);
				if (smoke_info.m_bDidSmokeEffect)
					should_draw_image = true;
			}

			if (should_draw_image)
				globals::draw_list->AddImage(icon.texture, image_start, image_end, ImVec2(0, 0), ImVec2(1, 1), imgui::ColorConvertFloat4ToU32(icon.tint));
			
			if (is_smoke)
			{
				const auto& smoke_info = std::get<entity_data::smoke_info_t>(grenade_info.specific_data);

				if (smoke_info.remaining_smoke_time > 0.f)
				{
					imgui::PushFont(render::fonts::esp_small);
					{
						ImVec2 center = pos.as_vec2();
						
						int time_remaining = static_cast<int>(smoke_info.remaining_smoke_time);
					
						std::string time_str = std::format("{:d}", time_remaining);
						auto text_size = imgui::CalcTextSize(time_str.c_str());
					
						float text_offset_y = 10.f;
						center.y += text_offset_y;

						outlined_text(time_str.c_str(), center - text_size / 2, IM_COL32_WHITE);
					}
					imgui::PopFont();
				}
			}
		}
	}

	//Dropped entities in the world, such as weapons, C4 and grenades.
	void dropped_entities(const entity_data::world_entity_info_t& entity_info)
	{
		const auto& icon = entity_info.icon;
		if (Vector pos; icon.texture && globals::world2screen(entity_info.m_vecOrigin, pos))
		{
			ImVec2 image_start = ImVec2(pos.x - icon.w / 2, pos.y - icon.h / 2 - 3.f);
			ImVec2 image_end = ImVec2(image_start.x + icon.w, image_start.y + icon.h);

			globals::draw_list->AddImage(icon.texture, image_start, image_end, ImVec2(0, 0), ImVec2(1, 1), imgui::ColorConvertFloat4ToU32(icon.tint));
		}
	}

	void render_entities()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(entity_data::entity_locker);

		m_entity_entry_data.clear();
		if (!entity_data::entity_entry_data.empty())
			std::copy(entity_data::entity_entry_data.begin(), entity_data::entity_entry_data.end(), std::back_inserter(m_entity_entry_data));

		for (auto& entry : m_entity_entry_data)
		{
			for (auto& bomb_info : entry.bomb_info)
				bomb_timer(bomb_info);

			for (auto& grenade_info : entry.grenade_info)
				grenade_projectiles(grenade_info);

			// for (auto& dropped_ents_info : entry.dropped_ent_info)
			// 	dropped_entities(dropped_ents_info);
		}
	}

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(entity_data::player_locker);
		
		//m_player_data.clear();
		if (!entity_data::player_entry_data.empty())
			m_player_data.assign(entity_data::player_entry_data.back().player_data.begin(), entity_data::player_entry_data.back().player_data.end());
		
		for (auto& data : m_player_data)
		{
			if (settings::esp::visible_only && (!data.flags.test(PLAYER_VISIBLE) || data.flags.test(PLAYER_IN_SMOKE)))
				continue;

			ImU32 box_color = ImColor(settings::esp::box_clr.x, settings::esp::box_clr.y, settings::esp::box_clr.z, settings::esp::box_clr.w);
			if (settings::esp::box_esp && data.bbox.IsValid())
			{
				globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), box_color, 0.f, 0, 1.2f);

				ImColor col_alpha0 = ImColor(settings::esp::box_clr.x, settings::esp::box_clr.y, settings::esp::box_clr.z, 0.f);
				ImColor col_alpha180 = ImColor(settings::esp::box_clr.x, settings::esp::box_clr.y, settings::esp::box_clr.z, 180 / 255.f);
				globals::draw_list->AddRectFilledMultiColor(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), col_alpha0, col_alpha0, col_alpha180, col_alpha180); 
				
				globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2() - ImVec2(1, 1), data.bbox.m_Maxs.as_vec2() + ImVec2(1, 1), ImColor(0, 0, 0, 180), 1.2f);
			}

			//health esp
			{
				float hp = static_cast<float>(data.m_iHealth);
				auto bottom_left = data.bbox.GetBottomLeft();
				float health_height = (data.bbox.GetHeight() * hp) / 100.0f;
				
				ImVec2 rect_start = ImVec2(bottom_left.x - 6, bottom_left.y);
				ImVec2 rect_end = ImVec2(bottom_left.x - 2, bottom_left.y - health_height);
				
				// Black background (fixed size from bottom to top)
				ImVec2 background_rect_start = ImVec2(bottom_left.x - 7, bottom_left.y);  // Extend left by 1 pixel
				ImVec2 background_rect_end = ImVec2(data.bbox.GetTopLeft().x - 1, data.bbox.GetTopLeft().y); // Full height of the bbox
				
				// Draw the black background (fixed size, from bottom to top)
				globals::draw_list->AddRectFilled(background_rect_start, background_rect_end, IM_COL32_BLACK);
				
				// Draw the health bar on top (colored, variable height)
				globals::draw_list->AddRectFilled(rect_start, rect_end, ImColor(255 - (hp * 2.55), hp * 2.55, 0, 255));
			}
			
			esp::name_esp(data, data.bbox);

			//Draw3DBox(data.bbox);
			//draw_fov(data);
			esp::bone_esp(data);
			esp::weapon_esp(data);
		}
	}
	
	void draw_fov(entity_data::player_data_t& data)
	{
		CCSPlayerController* controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
		if (!controller)
			return;

		CCSPlayerPawn* pawn = reinterpret_cast<CCSPlayerPawn*>(g::entity_system->GetEntityFromHandle(controller->m_hPawn()));
		if (!pawn)
			return;
		
		auto head_hitbox = &data.hitboxes[HITBOX_HEAD];
		if (!head_hitbox)
			return;
		
		Vector head_w2s;
		if (!globals::world2screen(head_hitbox->hitbox_pos, head_w2s))
			return;

		auto wpn = reinterpret_cast<CBasePlayerWeapon*>(g::entity_system->GetEntityFromHandle(pawn->m_pWeaponServices()->m_hActiveWeapon()));
		if (!wpn)
			return;
		
		auto weapon_config = settings::aimbot::weapon_configs[wpn->m_iItemDefinitionIndex()];
		float fov = weapon_config.fov;
		
		Vector hitbox_offset = head_hitbox->hitbox_pos + Vector(0, 0, 10);
		Vector offset_w2s;
		if (!globals::world2screen(hitbox_offset, offset_w2s))
			return;
		
		float screen_radius = head_w2s.dist_to(offset_w2s);
		
		screen_radius *= (fov / 10.0f);
		
		globals::draw_list->AddCircle(head_w2s.as_vec2(), screen_radius, IM_COL32(255, 255, 255, 255), 64, 1.5f);
	}
	
	void bone_esp(entity_data::player_data_t& data)
	{
		if (!settings::esp::bone_esp)
			return;

		for (const auto& x : data.bones_w2s)
		{
			if (x.got_bone && x.got_parent)
				globals::draw_list->AddLine(x.bone.as_vec2(), x.bone_parent.as_vec2(), ImColor(settings::esp::bone_clr.x, settings::esp::bone_clr.y, settings::esp::bone_clr.z, settings::esp::bone_clr.w));
		}
	}
	
	void name_esp(entity_data::player_data_t& data, const BBox_t& bbox)
	{
		if (!settings::esp::name_esp)
			return;
		
		Vector top_mid = bbox.GetTop();
		
		ImU32 color = ImColor(settings::esp::name_clr.x, settings::esp::name_clr.y, settings::esp::name_clr.z, settings::esp::name_clr.w);
		float y_padding = 20.0f;
		
		imgui::PushFont(render::fonts::esp);
		auto text_size = imgui::CalcTextSize(data.m_szPlayerName);
		imgui::PopFont();
		
		float text_size_mid = text_size.x * 0.5f;
	
		ImVec2 render_pos(top_mid.x - text_size_mid, top_mid.y - y_padding);
		
		imgui::PushFont(render::fonts::esp);
		outlined_text(data.m_szPlayerName, render_pos, IM_COL32(255, 255, 255, 255));
		imgui::PopFont();
	}
	
	void weapon_esp(entity_data::player_data_t& data)
	{
	    Vector bottom_mid = data.bbox.GetBottom();

	    const auto& icon = data.weapon_icon;
	    if (icon.texture)
	    {
	        auto controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
	        if (!controller)
	            return;

	        auto pawn = reinterpret_cast<CCSPlayerPawn*>(g::entity_system->GetEntityFromHandle(controller->m_hPawn()));
	        if (!pawn)
	            return;

	        // Calculate distance between the player's weapon and the camera
	        float distance = data.m_vecOrigin.dist_to(pawn->m_pGameSceneNode()->m_vecOrigin());

	        // Debug distance output
	        //g_Console->println("distance: %.1f", distance);

	        // Use a smooth falloff for scaling, using a more gradual approach
	        float scale_factor = 1.0f;

	        // If you're within a certain range, apply scaling
	        if (distance < 500.0f)
	        {
	            // Use a more exponential scaling for closer distances
	            scale_factor = 1.0f + std::clamp((500.0f - distance) / 500.0f, 0.0f, 0.2f);
	        }

	        // Debug output for scale_factor
	        //g_Console->println("scale_factor: %.2f", scale_factor);

	        // Apply the scale factor to width and height
	        float scaled_w = icon.w * scale_factor;
	        float scaled_h = icon.h * scale_factor;

	        // Ensure a minimum size so the icon doesn't disappear at long distances
	        scaled_w = std::max(scaled_w, 10.0f);
	        scaled_h = std::max(scaled_h, 10.0f);

	        // Debug output for scaled width and height
	        //g_Console->println("scaled_w: %.1f, scaled_h: %.1f", scaled_w, scaled_h);

	        // Position the icon based on bounding box center and scale
	        ImVec2 image_start = ImVec2(bottom_mid.x - scaled_w / 2, bottom_mid.y - scaled_h / 2 + 6.f);
	        ImVec2 image_end = ImVec2(image_start.x + scaled_w, image_start.y + scaled_h);

	        globals::draw_list->AddImage(icon.texture, image_start, image_end, ImVec2(0, 0), ImVec2(1, 1), imgui::ColorConvertFloat4ToU32(icon.tint));
	    }
	}
}
