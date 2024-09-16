#include "features.h"

namespace features::esp
{
	std::list<entity_data::player_data_t> m_player_data;
	
	void Draw3DBox(ABBox_t& bbox)
	{
		const int BOTTOM_RIGHT_BACK = 0;
		const int BOTTOM_RIGHT_FRONT = 1;
		const int BOTTOM_LEFT_BACK = 2;
		const int BOTTOM_LEFT_FRONT = 3;
		const int TOP_RIGHT_BACK = 4;
		const int TOP_RIGHT_FRONT = 5;
		const int TOP_LEFT_BACK = 6;
		const int TOP_LEFT_FRONT = 7;

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
			
			Vector head_pos = data.m_vecOrigin;
			head_pos.z += 75.f;

			bool got_origin = globals::world2screen(data.m_vecOrigin, origin_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			if (got_origin && got_head_pos)
				esp::name_esp(data, origin_out, head_pos_out);

			if (settings::visuals::m_bBoxEsp)
				globals::draw_list->AddRect(data.abbox.m_Mins.as_vec2(), data.abbox.m_Maxs.as_vec2(), IM_COL32(255, 0, 0, 255), 1.f, 15, 1.5f);
			
			Draw3DBox(data.abbox);

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
		
		Vector bone_pos_out;
		Vector bone_parent_pos_out;

		const auto& model = data.m_hModel;
		if (!model.IsValid())
			return;

		const auto& model_state = data.m_ModelState;

		static const auto& bone_count = model->BoneCount;

		for (int i = 0; i < bone_count; ++i) //bone_count returns some high bullshit number, with 28 it renders also correctly
		{
			const auto& flag = model->GetBoneFlags(i);
			if (!flag.HasFlag(static_cast<uint32_t>(FLAG_HITBOX)))
				continue;

			const auto& bone_parent_index = model->GetBoneParent(i);
			if (bone_parent_index == -1)
				continue;

			const auto& bones = model_state.bones[i];
			const auto& parent_bones = model_state.bones[bone_parent_index];

			const Vector& in_child = bones.position;
			const Vector& in_parent = parent_bones.position;

			bool got_bones = globals::world2screen(in_child, bone_pos_out);
			bool got_parents = globals::world2screen(in_parent, bone_parent_pos_out);

			auto p_out_v2 = bone_pos_out.as_vec2();
			auto bp_p_out_v2 = bone_parent_pos_out.as_vec2();

			if (got_bones && got_parents)
				globals::draw_list->AddLine(p_out_v2, bp_p_out_v2, ImColor(settings::visuals::m_fBoneColor.x, settings::visuals::m_fBoneColor.y, settings::visuals::m_fBoneColor.z, settings::visuals::m_fBoneColor.w));
		}
	}

	void name_esp(entity_data::player_data_t& data, const Vector& origin_out, const Vector& screen_head_pos)
	{
		if (!settings::visuals::m_bNameEsp)
			return;

		globals::draw_list->AddText(ImVec2(origin_out.x - 15, screen_head_pos.y - 14), ImColor(settings::visuals::m_fNameColor.x, settings::visuals::m_fNameColor.y, settings::visuals::m_fNameColor.z, settings::visuals::m_fNameColor.w), data.m_szPlayerName);
	}
}
