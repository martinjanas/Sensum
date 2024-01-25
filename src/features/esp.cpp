#include "features.h"

namespace features::esp
{
	std::list<entity_data::player_data_t> m_player_data;

	void render()
	{
		if (!g::engine_client->IsInGame())
			return;

		if (entity_data::player_instances.empty())
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

		for (auto& data : m_player_data) //TODO:Compute bbox in the future
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

			if (got_head_pos && got_old_origin)
				esp::box_esp(head_pos_out, player_pos_out);

			esp::bone_esp(data);

			//printf("l: %f, t: %f, r: %f, b: %f, w: %f, h: %f\n", data.bbox.left, data.bbox.top, data.bbox.right, data.bbox.bottom, data.bbox.width(), data.bbox.height());

			globals::draw_list->AddRect(ImVec2(data.bbox.left, data.bbox.top), ImVec2(data.bbox.right, data.bbox.bottom), IM_COL32_WHITE, 1.f, 15, 1.5f);
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

			bool got_bones = globals::world2screen(data.model_state->bones[i].position, bone_pos_out);
			bool got_parents = globals::world2screen(data.model_state->bones[bone_parent_index].position, bone_parent_pos_out);

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
