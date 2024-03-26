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
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].AsVec2(), bbox.m_Vertices[TOP_LEFT_BACK].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_FRONT].AsVec2(), bbox.m_Vertices[TOP_RIGHT_BACK].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].AsVec2(), bbox.m_Vertices[TOP_RIGHT_FRONT].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_BACK].AsVec2(), bbox.m_Vertices[TOP_RIGHT_BACK].AsVec2(), IM_COL32_WHITE);

		//Bottom face
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_FRONT].AsVec2(), bbox.m_Vertices[BOTTOM_LEFT_BACK].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_RIGHT_FRONT].AsVec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_FRONT].AsVec2(), bbox.m_Vertices[BOTTOM_RIGHT_FRONT].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[BOTTOM_LEFT_BACK].AsVec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].AsVec2(), IM_COL32_WHITE);

		//Left face
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_FRONT].AsVec2(), bbox.m_Vertices[BOTTOM_LEFT_FRONT].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_LEFT_BACK].AsVec2(), bbox.m_Vertices[BOTTOM_LEFT_BACK].AsVec2(), IM_COL32_WHITE);

		//Right face
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_FRONT].AsVec2(), bbox.m_Vertices[BOTTOM_RIGHT_FRONT].AsVec2(), IM_COL32_WHITE);
		globals::draw_list->AddLine(bbox.m_Vertices[TOP_RIGHT_BACK].AsVec2(), bbox.m_Vertices[BOTTOM_RIGHT_BACK].AsVec2(), IM_COL32_WHITE);
	}

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		if (entity_data::player_entry_data.empty())
			return;

		if (entity_data::locker.try_lock())
		{
			m_player_data.clear();
			std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_player_data));
			entity_data::locker.unlock();
		}

		static Vector head_pos_out;
		static Vector player_pos_out;
		static Vector origin_out;
	
		for (auto& data : m_player_data)
		{
			if (data.index == 0)
				continue;
			
			Vector head_pos = data.m_vOldOrigin;
			head_pos.z += 75.f;

			bool got_origin = globals::world2screen(data.m_vecOrigin, origin_out);
			bool got_old_origin = globals::world2screen(data.m_vOldOrigin, player_pos_out);
			bool got_head_pos = globals::world2screen(head_pos, head_pos_out);

			if (got_origin && got_head_pos)
				esp::name_esp(data, origin_out, head_pos_out);

			if (settings::visuals::m_bBoxEsp)
				globals::draw_list->AddRect(data.abbox.m_Mins.AsVec2(), data.abbox.m_Maxs.AsVec2(), IM_COL32(255, 0, 0, 255), 1.f, 15, 1.5f);
			
			//Draw3DBox(data.abbox);

			esp::bone_esp(data);
			//TODO: draw nodeToWorld->pos
			/*static Vector m_pos_out;
			if (data.scene_node && globals::world2screen(data.scene_node->m_nodeToWorld().m_pos, m_pos_out))
				globals::draw_list->AddCircleFilled(m_pos_out.AsVec2(), 2.f, IM_COL32_WHITE);*/
		}
	}

	void bone_esp(entity_data::player_data_t& data)
	{
		if (!settings::visuals::m_bBoneEsp)
			return;

		if (!data.model_state)
			return;

		if (!data.model.IsValid())
			return;

		static Vector bone_pos_out;
		static Vector bone_parent_pos_out;

		for (int i = 0; i <= data.model->BoneCount; ++i)
		{
			const auto flag = data.model->GetBoneFlags(i);

			if (!flag.HasFlag(static_cast<uint32_t>(FLAG_HITBOX)))
				continue;

			auto bone_parent_index = data.model->GetBoneParent(i);

			if (bone_parent_index == -1)
				continue;

			Vector in_child = data.model_state->bones[i].position;
			Vector in_parent = data.model_state->bones[bone_parent_index].position;

			bool got_bones = globals::world2screen(in_child, bone_pos_out);
			bool got_parents = globals::world2screen(in_parent, bone_parent_pos_out);

			if (got_bones && got_parents)
				globals::draw_list->AddLine(bone_pos_out.AsVec2(), bone_parent_pos_out.AsVec2(), ImColor(settings::visuals::m_fBoneColor.x, settings::visuals::m_fBoneColor.y, settings::visuals::m_fBoneColor.z, settings::visuals::m_fBoneColor.w));
		}
	}

	void box_esp(const Vector& head_pos_out, const Vector& player_pos_out)
	{
		if (!settings::visuals::m_bBoxEsp)
			return;

		float height = player_pos_out.y - head_pos_out.y;
		float width = height * 2.5f;

		width /= 10.f;

		globals::draw_list->AddRect(ImVec2(head_pos_out.x - width, head_pos_out.y), ImVec2(head_pos_out.x + width, player_pos_out.y), ImColor(settings::visuals::m_fBoxColor.x, settings::visuals::m_fBoxColor.y, settings::visuals::m_fBoxColor.z, settings::visuals::m_fBoxColor.w));
	}

	void name_esp(entity_data::player_data_t& data, const Vector& origin_out, const Vector& screen_head_pos)
	{
		if (!settings::visuals::m_bNameEsp)
			return;

		globals::draw_list->AddText(ImVec2(origin_out.x - 15, screen_head_pos.y - 14), ImColor(settings::visuals::m_fNameColor.x, settings::visuals::m_fNameColor.y, settings::visuals::m_fNameColor.z, settings::visuals::m_fNameColor.w), data.player_name);
	}
}
