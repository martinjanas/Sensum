#include "../render/render.h"
#include "features.h"
#include "../settings/globals.h"
#include "../helpers/imdraw.h"
#include "../helpers/entities.h"

namespace features
{
	std::mutex render_mutex;
	entities::player_data_t entities[MAX_PLAYERS];

	ImVec4 color;

	float per_x = 43.f;
	float per_y = 26.f;

	int index = 1;

	Vector origin;

	bool is_enabled()
	{
		if (!render::fonts::low_size || g::engine_client->IsConsoleVisible() || g::engine_client->IsDrawingLoadingImage())
			return false;

		return g::engine_client->IsInGame() && g::local_player && settings::visuals::player_info_box;
	}

	void player_infobox()
	{
		if (!is_enabled())
			return;

		if (entities::locker.try_lock())
		{
			memcpy(entities, entities::m_items.front().players, sizeof(entities));
			entities::locker.unlock();
		}

		auto player_resource = *g::player_resource;
		
		ImGuiStyle style = ImGui::GetStyle();

		ImVec2 old_minsize = style.WindowMinSize;
		style.WindowMinSize = ImVec2(0.f, 0.f);

		for (const auto& data : entities)
		{
			if (data.index == 0 || data.index == g::local_player->GetIndex())
				continue;

			if (data.team_num == g::local_player->m_iTeamNum() && !settings::misc::deathmatch)
				continue;

			if (index < 5)
			{
				if (data.is_alive)
					index += 1;
				else index -= 1;

				ImGui::SetNextWindowSize(ImVec2(215.f, per_y * index));
			}

			
		}
		ImGui::SetNextWindowBgAlpha(settings::visuals::player_info_box_alpha);

		ImGui::PushFont(render::fonts::low_size);
		{
			//ImGuiWindowFlags_AlwaysAutoResize
			if (ImGui::Begin("Player Info Box", &settings::visuals::player_info_box, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
			{
				render::columns(5);
				{
					ImGui::SetColumnWidth(-1, 40.f);
					ImGui::Text("Name");

					ImGui::NextColumn();

					ImGui::SetColumnWidth(-1, 25.f); //48
					ImGui::Text(" HP");

					ImGui::NextColumn();

					ImGui::SetColumnWidth(-1, 30.f); //45
					ImGui::Text("Level");

					ImGui::NextColumn();

					ImGui::SetColumnWidth(-1, 30.f);
					ImGui::Text("Wins");

					ImGui::NextColumn();

					ImGui::SetColumnWidth(-1, 30.f);
					ImGui::Text("Dist");
				}
				render::columns(1);

				for (const auto& data : entities)
				{
					if (data.index == 0 || data.index == g::local_player->GetIndex())
						continue;

					if (data.team_num == g::local_player->m_iTeamNum() && !settings::misc::deathmatch)
						continue;

					if (!player_resource)
						continue;

					render::columns(5);
					{
						if (data.is_alive)
							color = ImVec4(0.0f, 1.0f, 0.0f, 1.f);
						else color = ImVec4(1.0f, 0.f, 0.f, 1.f);

						ImGui::PushStyleColor(ImGuiCol_Text, color);

						ImGui::SetColumnWidth(-1, 40.f);
						ImGui::Text("%s", data.name.c_str());

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImVec4::White));

						ImGui::NextColumn();

						ImGui::SetColumnWidth(-1, 25.f);
						ImGui::Text("%i", data.m_iHealth);

						ImGui::NextColumn();

						ImGui::SetColumnWidth(-1, 30.f);
						ImGui::Text("%i", player_resource->GetLevel()[data.index] == -1 ? 0 : player_resource->GetLevel()[data.index]);

						ImGui::NextColumn();

						ImGui::SetColumnWidth(-1, 30.f);
						ImGui::Text("%i", player_resource->GetWins()[data.index]);

						ImGui::NextColumn();

						ImGui::SetColumnWidth(-1, 30.f);
						ImGui::Text("%1.f", data.origin.DistTo(g::local_player->m_vecOrigin()));
					}
					render::columns(1);
					ImGui::PopStyleColor(2);
				}
			}
			ImGui::End();
			style.WindowMinSize = old_minsize;
		}
		ImGui::PopFont();
	}
}