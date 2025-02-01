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
	
	void grenade_projectiles(const entity_data::grenade_info_t& grenade_info, ID3D11Device* device, ID3D11DeviceContext* context)
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

	void render_entities(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		if (!g::engine_client->IsInGame())
			return;

		std::shared_lock<std::shared_mutex> lock(entity_data::entity_locker);

		m_entity_entry_data.clear();
		if (!entity_data::entity_entry_data.empty())
			std::copy(entity_data::entity_entry_data.begin(), entity_data::entity_entry_data.end(), std::back_inserter(m_entity_entry_data));

		for (auto& entry : m_entity_entry_data)
		{
			for (auto& bomb_info : entry.bomb_info)
				bomb_timer(bomb_info);

			for (auto& grenade_info : entry.grenade_info)
				grenade_projectiles(grenade_info, device, context);

			for (auto& dropped_ents_info : entry.dropped_ent_info)
				dropped_entities(dropped_ents_info);
		}
	}

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::shared_lock<std::shared_mutex> lock(entity_data::player_locker);

		m_player_data.clear();
		if (!entity_data::player_entry_data.empty())
			std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_player_data));

		Vector head_pos_out;
		Vector origin_out;

		for (auto& data : m_player_data)
		{
			if (settings::esp::visible_only && (!data.flags.test(PLAYER_VISIBLE) || data.flags.test(PLAYER_IN_SMOKE)))
				continue;

			ImU32 box_color = ImColor(settings::esp::box_clr.x, settings::esp::box_clr.y, settings::esp::box_clr.z, settings::esp::box_clr.w);

			const Vector& head_pos = data.hitboxes[HITBOX_HEAD].hitbox_pos;

			bool got_origin = globals::world2screen(data.m_vecAbsOrigin, origin_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			if (settings::esp::box_esp)
				globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), box_color, 0.f, 0);
			
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

		// Scale the size/positioning to the bounding box width
		float width = bbox.GetWidth();

		// Get the middle-top position of the bounding box
		Vector top_mid = bbox.GetTopMid();

		// Text color
		ImU32 color = ImColor(settings::esp::name_clr.x, settings::esp::name_clr.y, settings::esp::name_clr.z, settings::esp::name_clr.w);

		// Vertical padding between the text and the top of the box
		float y_padding = 2.0f;

		// Calculate text size again with the adjusted font size
		imgui::PushFont(render::fonts::esp); // Set font
		auto adjusted_text_size = imgui::CalcTextSize(data.m_szPlayerName);
		imgui::PopFont();

		// Calculate the horizontal center of the text
		float text_size_mid = adjusted_text_size.x * 0.5f;

		// Determine the final render position
		ImVec2 render_pos(top_mid.x - text_size_mid, top_mid.y - adjusted_text_size.y - y_padding);

		imgui::PushFont(render::fonts::esp);
		outlined_text(data.m_szPlayerName, render_pos, IM_COL32(255, 255, 255, 255));
		imgui::PopFont();
	}

	//TODO: Calculate and set the image size to the bbox width, currently it looks ugly when changing distance
	void weapon_esp(entity_data::player_data_t& data)
	{
		Vector bottom_mid = data.bbox.GetBottomMid();

		const auto& icon = data.weapon_icon;
		if (icon.texture)
		{
			ImVec2 image_start = ImVec2(bottom_mid.x - icon.w / 2, bottom_mid.y - icon.h / 2 + 6.f);
			ImVec2 image_end = ImVec2(image_start.x + icon.w, image_start.y + icon.h);

			globals::draw_list->AddImage(icon.texture, image_start, image_end, ImVec2(0, 0), ImVec2(1, 1), imgui::ColorConvertFloat4ToU32(icon.tint));
		}
	}
}
