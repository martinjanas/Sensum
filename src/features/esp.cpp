#include "features.h"
#include "../render/render.h"
#include "../sdk/classes/CPlantedC4.h"
#include "../sdk/helpers/utils.h"
#include <format>

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

	//bomb_timer, grenade_projectiles and dropped_entities functions are in wip/prototype stage, they will be redone properly later
	void bomb_timer(const entity_data::bomb_info_t& bomb_info)
	{
		auto bomb_time = bomb_info.m_flC4Blow - g::global_vars->m_curtime;
		bomb_time = std::clamp<float>(bomb_time, 0.f, 40.f);
		if (bomb_time <= 0.0f)
			return;

		Vector pos;
		if (globals::world2screen(bomb_info.m_vecAbsOrigin, pos))
		{
			ImGui::PushFont(render::fonts::header_buttons);
			globals::draw_list->AddText(pos.as_vec2(), IM_COL32_BLACK, std::format("Time: {:.1f}", bomb_time).c_str());
			ImGui::PopFont();
		}
	}

	void grenade_projectiles(const entity_data::grenade_info_t& grenade_info)
	{
		Vector pos;
		if (globals::world2screen(grenade_info.m_vecOrigin, pos))
			globals::draw_list->AddRect({ pos.x + 10.f, pos.y + 10.f }, { pos.x - 10.f, pos.y - 10.f }, IM_COL32_WHITE);
	}

	//Dropped entities in the world, such as weapons, C4 and grenades.
	void dropped_entities(const entity_data::world_entity_info_t& entity_info)
	{
		Vector pos;
		if (globals::world2screen(entity_info.m_vecOrigin, pos))
		{
			globals::draw_list->AddRect({ pos.x + 10.f, pos.y + 10.f }, { pos.x - 10.f, pos.y - 10.f }, IM_COL32_WHITE);

			ImGui::PushFont(render::fonts::header_buttons);
			globals::draw_list->AddText(pos.as_vec2(), IM_COL32_WHITE, entity_info.name);
			ImGui::PopFont();
		}
	}

	void render_entities()
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
				grenade_projectiles(grenade_info);

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

		static Vector head_pos_out;
		static Vector origin_out;

		for (auto& data : m_player_data)
		{
			if (settings::esp::visible_only && (!data.flags.test(PLAYER_VISIBLE) || data.flags.test(PLAYER_IN_SMOKE)))
				continue;

			const Vector& head_pos = data.hitboxes[HITBOX_HEAD].hitbox_pos;

			bool got_origin = globals::world2screen(data.m_vecAbsOrigin, origin_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			if (settings::esp::box_esp)
				globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), ImColor(settings::esp::box_clr.x, settings::esp::box_clr.y, settings::esp::box_clr.z, settings::esp::box_clr.w), 0.f, 0);
			
			esp::name_esp(data, data.bbox);

			//Draw3DBox(data.bbox);

			esp::bone_esp(data);

			/*if (!data.hitboxes.empty())
			{
				Vector hitbox_w2s;	
				for (auto& hitbox_data : data.hitboxes)
				{
					Vector hitbox_pos = hitbox_data.hitbox_pos;

					//if (globals::world2screen(hitbox_pos, hitbox_w2s))
						//globals::draw_list->AddText(hitbox_w2s.as_vec2(), IM_COL32_WHITE, utils::hitbox_index_to_name(hitbox_data.index));

					if (globals::world2screen(hitbox_pos, hitbox_w2s))
						globals::draw_list->AddCircle(hitbox_w2s.as_vec2(), 8.f, IM_COL32_WHITE, 255);
				}
			}*/
		}
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

		Vector top_mid = bbox.GetTopMid();
		ImU32 color = ImColor(settings::esp::name_clr.x, settings::esp::name_clr.y, settings::esp::name_clr.z, settings::esp::name_clr.w);

		Vector screen_top_left = bbox.GetTopLeft();
		Vector screen_top_right = bbox.GetTopRight();

		static float base_font_size = 16.f;

		auto projected_box_width = screen_top_right.x - screen_top_left.x;
		float scale_factor = projected_box_width / (base_font_size * 3.f);
		float font_size = scale_factor * base_font_size;

		font_size = std::clamp<float>(font_size, 12.f, 24.f);

		// Calculate the text size and position
		auto text_size = render::fonts::header_buttons->CalcTextSizeA(font_size, FLT_MAX, 0.0f, data.m_szPlayerName);
		auto text_size_mid = text_size.x * 0.5f;
		auto y_padding = 5.f;

		//g_Console->println("Scale Factor: %.1f", scale_factor);
		
		ImVec2 render_pos(top_mid.x - text_size_mid, top_mid.y - text_size.y - y_padding);

		globals::draw_list->AddText(render::fonts::header_buttons, font_size, render_pos, color, data.m_szPlayerName);
	}
}
