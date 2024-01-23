#include "render.h"
#include "../settings/options.hpp"
#include "../settings/globals.h"

namespace render
{
	namespace spectators
	{
		void show()
		{
			if (!render::fonts::spectators || !g::local_player)
				return;

			c_base_player* player = g::local_player;
			if (player->m_hObserverTarget())
				player = c_base_player::GetEntityFromHandle<c_base_player>(player->m_hObserverTarget());

			if (!player || !player->IsPlayer())
				return;

			auto offset_y = 5.f;
			const auto screen_size = ImGui::GetIO().DisplaySize;
			const auto color = ImGui::GetColorU32(player->IsFlashed() ? ImVec4::Black : ImVec4::White);

			const auto max_clients = g::engine_client->GetMaxClients();
			for (int i = 1; i <= max_clients; i++)
			{
				auto entity = c_base_player::GetPlayerByIndex(i);
				if (!entity || !entity->IsPlayer() || entity->IsDormant() || entity->IsAlive() || entity == player || entity == g::local_player)
					continue;

				const auto observer_target = c_base_player::GetEntityFromHandle(entity->m_hObserverTarget());
				if (!observer_target || observer_target != player)
					continue;

				auto info = entity->GetPlayerInfo();
				if (!info.fakeplayer)
				{
					const auto text_size = ImGui::CalcTextSize(info.szName);
					const auto pos = ImVec2(screen_size.x - text_size.x - 5.f, offset_y);

					globals::draw_list->AddText(pos, color, info.szName);

					offset_y += text_size.y + 5.f;
				}
			}
		}
	}
}