#include "features.h"
#include "../render/menu/main_window.h"

namespace features::esp
{
	std::list<entity_data::player_data_t> m_player_data;

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

	const char* hitbox_index_to_name(int index)
	{
		switch (index)
		{
		case HITBOX_HEAD:
			return "Head";
		case HITBOX_NECK:
			return "Neck";
		case HITBOX_PELVIS:
			return "Pelvis";
		case HITBOX_BELLY:
			return "Belly";
		case HITBOX_THORAX:
			return "Thorax";
		case HITBOX_LOWER_CHEST:
			return "Lower Chest";
		case HITBOX_UPPER_CHEST:
			return "Upper Chest";
		case HITBOX_RIGHT_THIGH:
			return "Right Thigh";
		case HITBOX_LEFT_THIGH:
			return "Left Thigh";
		case HITBOX_RIGHT_CALF:
			return "Right Calf";
		case HITBOX_LEFT_CALF:
			return "Left Calf";
		case HITBOX_RIGHT_FOOT:
			return "Right Foot";
		case HITBOX_LEFT_FOOT:
			return "Left Foot";
		case HITBOX_RIGHT_HAND:
			return "Right Hand";
		case HITBOX_LEFT_HAND:
			return "Left Hand";
		case HITBOX_RIGHT_UPPER_ARM:
			return "Right Upper Arm";
		case HITBOX_RIGHT_FOREARM:
			return "Right Forearm";
		case HITBOX_LEFT_UPPER_ARM:
			return "Left Upper Arm";
		case HITBOX_LEFT_FOREARM:
			return "Left Forearm";
		}
	}

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		if (entity_data::locker.try_lock())
		{
			std::lock_guard<std::mutex> lock(entity_data::locker, std::adopt_lock);

			m_player_data.clear();

			if (!entity_data::player_entry_data.empty())
				std::ranges::copy(entity_data::player_entry_data.back().player_data, std::back_inserter(m_player_data));
		}

		static Vector head_pos_out;
		static Vector origin_out;

		for (auto& data : m_player_data)
		{
			if (!data.flags.test(PLAYER_VISIBLE) || data.flags.test(PLAYER_IN_SMOKE))
				continue;

			Vector head_pos = data.m_vecAbsOrigin;
			head_pos.z += 75.f;

			bool got_origin = globals::world2screen(data.m_vecAbsOrigin, origin_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			if (settings::visuals::m_bBoxEsp)
				globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), ImColor(settings::visuals::m_fBoxColor.x, settings::visuals::m_fBoxColor.y, settings::visuals::m_fBoxColor.z, settings::visuals::m_fBoxColor.w), settings::visuals::box_rounding, 0, settings::visuals::box_thickness);
			
			esp::name_esp(data, data.bbox);

			Draw3DBox(data.bbox);

			esp::bone_esp(data);

			//if (!data.hitboxes.empty())
			//{
			//	Vector hitbox_w2s;	
			//	for (auto& hitbox_data : data.hitboxes)
			//	{
			//		Vector hitbox_pos = hitbox_data.hitbox_pos;

			//		if (globals::world2screen(hitbox_pos, hitbox_w2s))
			//			globals::draw_list->AddText(hitbox_w2s.as_vec2(), IM_COL32_WHITE, hitbox_index_to_name(hitbox_data.index));

			//		/*if (globals::world2screen(hitbox_pos, hitbox_w2s))
			//			globals::draw_list->AddCircle(hitbox_w2s.as_vec2(), 8.f, IM_COL32_WHITE, 255);*/
			//	}
			//}
		}
	}

	void bone_esp(entity_data::player_data_t& data)
	{
		if (!settings::visuals::m_bBoneEsp)
			return;

		for (const auto& x : data.bones_w2s)
		{
			if (x.got_bone && x.got_parent)
				globals::draw_list->AddLine(x.bone.as_vec2(), x.bone_parent.as_vec2(), ImColor(settings::visuals::m_fBoneColor.x, settings::visuals::m_fBoneColor.y, settings::visuals::m_fBoneColor.z, settings::visuals::m_fBoneColor.w));
		}
	}

	void name_esp(entity_data::player_data_t& data, const BBox_t& bbox)
	{
		if (!settings::visuals::m_bNameEsp)
			return;

		Vector top_mid = bbox.GetTopMid();
		ImU32 color = ImColor(settings::visuals::m_fNameColor.x, settings::visuals::m_fNameColor.y, settings::visuals::m_fNameColor.z, settings::visuals::m_fNameColor.w);

		Vector screen_top_left = bbox.GetTopLeft();
		Vector screen_top_right = bbox.GetTopRight();

		static float base_font_size = 16.f;

		auto projected_box_width = screen_top_right.x - screen_top_left.x;
		float scale_factor = projected_box_width / (base_font_size * 3.f);
		float font_size = scale_factor * base_font_size;

		font_size = std::clamp<float>(font_size, 12.f, 24.f);

		// Calculate the text size and position
		auto text_size = main_window::esp->CalcTextSizeA(font_size, FLT_MAX, 0.0f, data.m_szPlayerName);
		auto text_size_mid = text_size.x * 0.5f;
		auto y_padding = 5.f;

		//g_Console->println("Scale Factor: %.1f", scale_factor);
		
		ImVec2 render_pos(top_mid.x - text_size_mid, top_mid.y - text_size.y - y_padding);

		globals::draw_list->AddText(main_window::esp, font_size, render_pos, color, data.m_szPlayerName);
	}
}
