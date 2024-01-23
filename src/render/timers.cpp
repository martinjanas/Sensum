#include "render.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/imdraw.h"
#include "../helpers/entities.h"
#include "../features/features.h"
#include "..//helpers/notifies.h"

namespace render
{
	const char* get_time_as_string()
	{
		static tm now;
		static char t_now[16];

		std::time_t t = std::time(0);

		localtime_s(&now, &t);
		strftime(t_now, sizeof(t_now), xorstr_("%X"), &now);

		return t_now;
	}

	namespace timers
	{
		auto m_Framerate = 0.f;

		ConVar* cl_hud_playercount_pos = nullptr;

		decltype(entities::m_local) m_local;
		entities::player_data_t m_players[MAX_PLAYERS];
	
		void render_panel()
		{
			if (g::engine_client->IsDrawingLoadingImage())
				return;

			if (!cl_hud_playercount_pos)
				cl_hud_playercount_pos = g::cvar->find("cl_hud_playercount_pos");

			if (!cl_hud_playercount_pos)
				return;

			ImGui::PushFont(render::fonts::low_size);

			const auto screen_size = ImGui::GetIO().DisplaySize;

			const auto is_top = cl_hud_playercount_pos->GetBool();

			const auto start_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
			const auto end_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f));

			static const auto fps_text_size = ImGui::CalcTextSize("FPS");

			const auto start_pos = ImVec2(screen_size.x / 2.f - 240.f, (is_top ? fps_text_size.y + 4.f : screen_size.y - fps_text_size.y - 4.f));
			const auto end_pos = ImVec2(screen_size.x / 2.f + 240.f, is_top ? 0.f : screen_size.y);

			if (entities::locker.try_lock())
			{
				memcpy(m_players, entities::m_items.front().players, sizeof(m_players));
				entities::locker.unlock();
			}

			globals::draw_list->AddRectFilledMultiColor(
				start_pos, ImVec2(start_pos.x + 240.f, end_pos.y), start_color, end_color, end_color, start_color
			);

			globals::draw_list->AddRectFilledMultiColor(
				ImVec2(start_pos.x + 240.f, start_pos.y), end_pos, end_color, start_color, start_color, end_color
			);

			char fps[256];
			sprintf(fps, "%4i", (int)(1.0f / m_Framerate));

			auto txt_size = ImGui::CalcTextSize(fps);
			auto text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);

			const auto fps_color = g::global_vars->interval_per_tick * 0.8f < g::global_vars->absoluteframetime ? ImVec4(1.f, 0.37f, 0.15f, 1.f) : ImVec4(0.64f, 0.82f, 0.45f, 1.f);

			if (!m_local.bomb_has_been_planted)
			{
				imdraw::outlined_text(fps, text_pos, ImGui::GetColorU32(fps_color));

				txt_size = ImGui::CalcTextSize("FPS");
				text_pos = ImVec2(start_pos.x + 240.f + 2.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
				imdraw::outlined_text("FPS", text_pos, ImGui::GetColorU32(ImVec4::White));

				const char* current_time = get_time_as_string();

				auto current_time_txt_size = ImGui::CalcTextSize(current_time);

				imdraw::outlined_text(current_time, ImVec2(text_pos.x + current_time_txt_size.x, text_pos.y), ImGui::GetColorU32(ImVec4::White));
			}

			if (m_local.is_matchmaking) //TODO: Recode using std::format
			{
				char bomb_time[16];
				sprintf_s(bomb_time, "%.2f", m_local.bomb_time);

				const auto bomb_color = m_local.bomb_time > 0.f ? ImVec4(0.88f, 0.82f, 0.45f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f);

				char defuse_time[16];
				sprintf_s(defuse_time, "%.2f", m_local.defuse_time);

				char bomb_location[256];
				sprintf_s(bomb_location, "%s", m_local.bombsite_index ? "BOMB: B" : "BOMB: A");

				char hp[256];
				sprintf_s(hp, "%d", m_local.hp);

				static const auto mp_c4timer = g::cvar->find("mp_c4timer");

				if (m_local.is_bomb_planted && (m_local.bomb_time > 0.f && m_local.bomb_time <= mp_c4timer->GetFloat()))
				{
					const auto bomb_text_size = ImGui::CalcTextSize("BOMB");
					txt_size = ImGui::CalcTextSize(bomb_time);
					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - bomb_text_size.x - 4.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
					imdraw::outlined_text(bomb_time, text_pos, ImGui::GetColorU32(bomb_color));

					txt_size = ImGui::CalcTextSize(defuse_time);
					text_pos = ImVec2(start_pos.x + 319.f - txt_size.x - 4.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);

					const auto defuse_color = m_local.defuse_time > 0.f ? ImVec4(0.f, 1.f, 0.15f, 1.f) : ImVec4(1.f, 1.f, 1.f, 1.f);
					imdraw::outlined_text(defuse_time, text_pos, ImGui::GetColorU32(defuse_color));

					const auto speed_text_size = ImGui::CalcTextSize("DEFUSE");

					text_pos = ImVec2(start_pos.x + 240.f + 2.f + 25.f + 30.f + 22.f, !is_top ? end_pos.y - 2.f - speed_text_size.y : end_pos.y + 2.f);
					imdraw::outlined_text("DEFUSE", text_pos, ImGui::GetColorU32(ImVec4::White));

					text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x - 25.f - 30.f - bomb_text_size.x, !is_top ? end_pos.y - 2.f - bomb_text_size.y : end_pos.y + 2.f);
					imdraw::outlined_text(bomb_location, text_pos, ImGui::GetColorU32(ImVec4::White));

					txt_size = ImGui::CalcTextSize(hp);

					if (m_local.is_alive) 
					{
						text_pos = ImVec2(start_pos.x + 240.f - 2.f - txt_size.x, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
						imdraw::outlined_text(m_local.hp <= 0 ? "DEAD" : hp, text_pos, ImGui::GetColorU32(m_local.hp <= 0 ? ImVec4::Red : bomb_color));

						if (m_local.hp > 0)
						{
							text_pos = ImVec2(start_pos.x + 240.f + 2.f, !is_top ? end_pos.y - 2.f - txt_size.y : end_pos.y + 2.f);
							imdraw::outlined_text("HP", text_pos, ImGui::GetColorU32(ImVec4::White));
						}
					}
				}
			}
			ImGui::PopFont();
		}

		void show()
		{
			m_Framerate = 0.9 * m_Framerate + (1.0 - 0.9) * g::global_vars->absoluteframetime;

			if (!g::local_player || !render::fonts::low_size)
				return;

			if (entities::local_mutex.try_lock())
			{
				m_local = entities::m_local;

				entities::local_mutex.unlock();
			}

			render_panel();
		}
	}
}