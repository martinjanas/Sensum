#include "nade_helper.h"
#include "../render/render.h"
#include "../helpers/imdraw.h"

#include <map>

std::unordered_map<std::string, std::vector<grenade_data_t>> data =
{
	{ "mirage", grenade_data_mirage },
	{ "inferno", grenade_data_inferno },
	{ "dust2", grenade_data_dust2 },
	{ "nuke", grenade_data_nuke },
	{ "overpass", grenade_data_overpass },
	{ "train", grenade_data_train },
	{ "cache", grenade_data_cache },
	{ "cbble", grenade_data_cobblestone }
};

namespace visuals
{
	void SmokeHelperAimbot(CUserCmd* cmd)
	{
		if (!g::local_player)
			return;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str())) //TODO: Convert to hash.
			{
				for (auto& it : x.second)
				{
					if (g::local_player->m_hActiveWeapon())
					{
						auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);
						if (g::local_player->m_hActiveWeapon()->IsSmoke() && dist <= 70)
						{
							QAngle angle = QAngle(it.ang.x, it.ang.y, it.ang.z);
							float fov = math::GetFovToPlayer(cmd->viewangles, angle);

							math::smooth(16.f, cmd->viewangles, angle, angle, false);

							angle.NormalizeClamp();

							if (fov <= 2.f)
							{
								if (cmd->buttons & (IN_ATTACK | IN_ATTACK2))
									g::engine_client->SetViewAngles(angle);
							}
						}
					}
				}
			}
		}
	}

	void RenderCircle()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::smoke_helper)
			return;

		if (g::engine_client->IsConsoleVisible())
			return;

		static Vector out;

		int x, y;
		g::engine_client->GetScreenSize(x, y);

		int cy = y / 2;
		int cx = x / 2;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str())) //TODO: Convert to hash.
			{
				for (auto& it : x.second)
				{
					if (!g::local_player->m_hActiveWeapon())
						continue;

					if (!g::local_player->m_hActiveWeapon()->IsSmoke())
						continue;

					float distance = g::local_player->m_vecOrigin().DistTo(it.pos);

					if (distance > 70.f)
						continue;
			
					Vector crosshair_pos = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
					Vector circle_pos = utils::CalcHelpPos(it.pos);

					if (!math::world2screen(crosshair_pos, out))
						continue;

					globals::draw_list->AddCircle(ImVec2(out.x, out.y), 9.f, IM_COL32(255, 255, 255, 255));
					globals::draw_list->AddCircleFilled(ImVec2(out.x, out.y), 8.f, IM_COL32(255, 255, 0, 255));
					globals::draw_list->AddCircleFilled(ImVec2(out.x - circle_pos.x, out.y - circle_pos.y), 2.f, IM_COL32(0, 0, 0, 255));
					imdraw::outlined_text(it.info.c_str(), ImVec2(out.x + 12.f, out.y - 7.f), IM_COL32(255, 255, 255, 255));
					globals::draw_list->AddLine(ImVec2(cx, cy), ImVec2(out.x, out.y), IM_COL32(0, 0, 0, 127), 2.f);
					globals::draw_list->AddCircle(ImVec2(cx, cy), 10.f, utils::to_im32(Color::White), 255);
				}
            }
		}
	}

	void drawring_3d()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::smoke_helper)
			return;

		if (g::engine_client->IsConsoleVisible())
			return;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str())) //TODO: Conver to hash.
			{
				for (auto& it : x.second)
				{
					Vector ppos = it.pos;

					auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsSmoke() && dist <= 500.f)
						{
							globals::draw_list->AddRing3D(Vector(ppos.x, ppos.y, ppos.z - it.ring_height), 15.f, 255, IM_COL32_WHITE, 1.0f);
						}
					}
				}
			}
		}
	}

	void RenderInfo()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::smoke_helper)
			return;

		if (g::engine_client->IsConsoleVisible())
			return;

		Vector InfoPosScreen;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str())) //TODO: Convert to hash.
			{
				for (auto& it : x.second)
				{
					Vector InfoPos = it.pos;

					float dist = g::local_player->m_vecOrigin().DistTo(it.pos);
					auto size = ImGui::CalcTextSize(it.info.c_str());

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsSmoke() && dist <= 500.f && math::world2screen(InfoPos, InfoPosScreen))
						{
							imdraw::outlined_text(it.info.c_str(), ImVec2(InfoPosScreen.x - it.info.length() - 15.f, InfoPosScreen.y - 75.f), IM_COL32(255, 255, 255, 255));
						}
					}
				}
			}
		}
	}
}