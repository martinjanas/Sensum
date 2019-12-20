#include "../render.h"
#include "../../globals.h"

namespace render
{
	namespace menu
	{
		const auto blue = ImVec4(0.05f, 0.4f, 0.7f, 1.f);
		const auto orange = ImVec4(0.7f, 0.4f, 0.05f, 1.f);

		struct player_info_t
		{
			bool is_ct_team;
			std::string name;
			int wins;
			int rank;
			int money;
			int id;
			int level;
		};

		std::vector<player_info_t> list;

		void render_players(const std::vector<player_info_t>& list, const bool& ct_team)
		{
			auto is_first = true;
			const std::vector<const char*> ranks =
			{
				strings::players_rank0.c_str(),
				strings::players_rank1.c_str(),
				strings::players_rank2.c_str(),
				strings::players_rank3.c_str(),
				strings::players_rank4.c_str(),
				strings::players_rank5.c_str(),
				strings::players_rank6.c_str(),
				strings::players_rank7.c_str(),
				strings::players_rank8.c_str(),
				strings::players_rank9.c_str(),
				strings::players_rank10.c_str(),
				strings::players_rank11.c_str(),
				strings::players_rank12.c_str(),
				strings::players_rank13.c_str(),
				strings::players_rank14.c_str(),
				strings::players_rank15.c_str(),
				strings::players_rank16.c_str(),
				strings::players_rank17.c_str(),
				strings::players_rank18.c_str(),
			};

			for (const auto& item : list)
			{
				if ((ct_team && !item.is_ct_team) || (!ct_team && item.is_ct_team))
					continue;

				if (is_first)
				{
					is_first = false;
					ImGui::Separator();
				}

				columns(5);

				ImGui::SetColumnWidth(-1, 200.f);
				ImGui::PushStyleColor(ImGuiCol_Text, item.is_ct_team ? blue : orange);

				ImGui::Text(item.name.c_str());

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("$%d", item.money);

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("%d", item.wins);

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text("%d", globals::team_damage[item.id]);

				ImGui::NextColumn();

				if (item.rank == 0)
					ImGui::Text("[%i] %s", item.level, ranks[item.rank]);
				else
					ImGui::Text("[%i] %s", item.level, ranks[item.rank]);

				ImGui::PopStyleColor();

				columns(1);
			}
		}

		void players_tab()
		{
			columns(5);
			{
				ImGui::SetColumnWidth(-1, 200.f);
				ImGui::Text(strings::players_name.c_str());

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(strings::players_money.c_str());

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(strings::players_wins.c_str());

				ImGui::NextColumn();

				ImGui::SetColumnWidth(-1, 60.f);
				ImGui::Text(strings::players_damage.c_str());

				ImGui::NextColumn();

				ImGui::Text(strings::players_rank.c_str());
			}
			columns(1);

			list.clear();

			auto player_resource = *interfaces::player_resource;
			if (!player_resource)
				return;

			for (int i = 1; i < interfaces::engine_client->GetMaxClients(); ++i)
			{
				auto* player = c_base_player::GetPlayerByIndex(i);
				if (!player || !player->IsPlayer())
					continue;

				if (player->m_iTeamNum() != team::team_ct && player->m_iTeamNum() != team::team_t)
					continue;

				const auto info = player->GetPlayerInfo();
				if (info.fakeplayer)
					continue;

				list.push_back({
					player->m_iTeamNum() == team::team_ct,
					info.szName,
					player_resource->GetWins()[player->GetIndex()],
					player_resource->GetRank()[player->GetIndex()],
					player->m_iAccount(),
					player->GetPlayerInfo().userId,
					player_resource->GetLevel()[player->GetIndex()]
					});
			}

			render_players(list, false);
			render_players(list, true);
		}
	}
}
