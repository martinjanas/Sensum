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

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		if (entity_data::locker.try_lock())
		{
			std::lock_guard<std::mutex> lock(entity_data::locker, std::adopt_lock);

			m_player_data.clear();

			if (!entity_data::player_entry_data.empty())
				std::ranges::copy(entity_data::player_entry_data.front().player_data, std::back_inserter(m_player_data));
		}

		static Vector head_pos_out;
		static Vector origin_out;

		for (auto& data : m_player_data)
		{
			if (data.m_iPlayerIndex == 0)
				continue;
			
			/*if (!data.is_visible)
				continue;*/

			Vector head_pos = data.m_vecAbsOrigin;
			head_pos.z += 75.f;

			bool got_origin = globals::world2screen(data.m_vecAbsOrigin, origin_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			/*if (got_origin && got_head_pos)
				esp::name_esp(data, origin_out, head_pos_out);*/

			if (settings::visuals::m_bBoxEsp)
				globals::draw_list->AddRect(data.bbox.GetTopLeft().as_vec2(), data.bbox.GetBottomRight().as_vec2(), ImColor(settings::visuals::m_fBoxColor.x, settings::visuals::m_fBoxColor.y, settings::visuals::m_fBoxColor.z, settings::visuals::m_fBoxColor.w), 1.f, 15, 1.5f);
				//globals::draw_list->AddRect(data.bbox.m_Mins.as_vec2(), data.bbox.m_Maxs.as_vec2(), ImColor(settings::visuals::m_fBoxColor.x, settings::visuals::m_fBoxColor.y, settings::visuals::m_fBoxColor.z, settings::visuals::m_fBoxColor.w), 1.f, 15, 1.5f);
			
			esp::name_esp(data, data.bbox);

			//Draw3DBox(data.bbox);

			esp::bone_esp(data);

			/*if (!data.hitboxes.empty())
			{
				Vector hitbox_w2s;
				
				for (auto& hitbox_data : data.hitboxes)
				{
					Vector hitbox_pos = hitbox_data.hitbox_pos;

					if (globals::world2screen(hitbox_pos, hitbox_w2s))
						globals::draw_list->AddCircle(hitbox_w2s.as_vec2(), 8.f, IM_COL32_WHITE, 255);
				}
			}*/
		}
	}

	void bone_esp(entity_data::player_data_t& data)
	{
		if (!settings::visuals::m_bBoneEsp)
			return;
		
		Vector bone_out;
		Vector bone_parent_out;

		const auto& model = data.m_hModel;
		if (!model.IsValid())
			return;

		const auto& model_state = data.m_ModelState;

		const Vector neck_chest_delta = model_state.bones[EBones::BONE_NECK].position - model_state.bones[EBones::BONE_SPINE_3].position;
		const Vector chest_neck_midpoint = model_state.bones[EBones::BONE_SPINE_3].position + (neck_chest_delta * 0.5f);

		for (int i = 0; i < EBones::BONE_MAX; ++i)
		{
			const auto& flag = model->GetBoneFlags(i);
			if (!flag.HasFlag(static_cast<uint32_t>(FLAG_HITBOX)))
				continue;

			const auto& bone_parent_index = model->GetBoneParent(i);
			if (bone_parent_index == -1)
				continue;

			const auto& bones = model_state.bones[i];
			const auto& parent_bones = model_state.bones[bone_parent_index];

			Vector bone_pos = bones.position;
			Vector parent_pos = parent_bones.position;

			Vector delta_child = bones.position - chest_neck_midpoint;
			Vector delta_parent = parent_bones.position - chest_neck_midpoint;

			if (delta_parent.length() < 9.0f && delta_child.length() < 9.0f)
				parent_pos = chest_neck_midpoint;

			if (i == EBones::BONE_SPINE_2)
				bone_pos = chest_neck_midpoint;

			if (abs(delta_child.z) < 5.0f && delta_parent.length() < 5.0f && delta_child.length() < 5.0f || i == EBones::BONE_SPINE_3)
				continue;

			bool got_bones = globals::world2screen(bone_pos, bone_out);
			bool got_parents = globals::world2screen(parent_pos, bone_parent_out);

			if (got_bones && got_parents)
				globals::draw_list->AddLine(bone_out.as_vec2(), bone_parent_out.as_vec2(), ImColor(settings::visuals::m_fBoneColor.x, settings::visuals::m_fBoneColor.y, settings::visuals::m_fBoneColor.z, settings::visuals::m_fBoneColor.w));
		}
	}

	void name_esp(entity_data::player_data_t& data, const BBox_t& bbox)
	{
		// Check if Name ESP is enabled in settings
		if (!settings::visuals::m_bNameEsp)
			return;

		Vector top_mid = bbox.GetTopMid();
		ImU32 color = ImColor(settings::visuals::m_fNameColor.x, settings::visuals::m_fNameColor.y, settings::visuals::m_fNameColor.z, settings::visuals::m_fNameColor.w);

		Vector screen_top_left = bbox.GetTopLeft();
		Vector screen_top_right = bbox.GetTopRight();

		auto projected_box_width = screen_top_right.x - screen_top_left.x;

		static const float bbox_size_x = 32.f;

		float scale_factor = projected_box_width / bbox_size_x;

		// Calculate the text size and position
		auto text_size = ImGui::CalcTextSize(data.m_szPlayerName);
		auto text_size_mid = text_size.x * 0.5f;
		auto y_padding = 5.f;
		
		//not right: omfg
		ImVec2 render_pos(top_mid.x + (text_size_mid * scale_factor), top_mid.y - (text_size.y * scale_factor) - y_padding);

		/*ImVec2 render_pos(screen_top_left.x + (projected_box_width * 0.5f) - text_size_mid,
			screen_top_left.y - text_size.y - y_padding);*/

		// Draw the player name with the adjusted scale factor
		globals::draw_list->AddText(main_window::esp, scale_factor, render_pos, color, data.m_szPlayerName);
	}
}
