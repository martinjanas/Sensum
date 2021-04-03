#include "nade_helper.h"
#include <map>

std::map<std::string, std::vector<grenade_data_t>> data =
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
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
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

							if (fov <= 4.f)
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

		Vector CrosshairScreen;

		int x, y;
		g::engine_client->GetScreenSize(x, y);

		int cy = y / 2;
		int cx = x / 2;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
			{
				for (auto& it : x.second)
				{
					Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
					Vector TCircleOfst = utils::CalcHelpPos(it.pos);

					auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);
					auto size = ImGui::CalcTextSize(it.info.c_str());

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsSmoke() && dist <= 70 && math::world2screen(Crosshair, CrosshairScreen))
						{
							globals::draw_list->AddRectFilled(ImVec2(CrosshairScreen.x - 20.f, CrosshairScreen.y - 10.f), ImVec2(CrosshairScreen.x + size.x + 25.f, CrosshairScreen.y + 12.f), IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddRectFilled(ImVec2(CrosshairScreen.x - 20.f, CrosshairScreen.y - -10.f), ImVec2(CrosshairScreen.x + size.x + 25.f, CrosshairScreen.y + 22.f), IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddCircle(ImVec2(CrosshairScreen.x, CrosshairScreen.y), 9.f, IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddCircleFilled(ImVec2(CrosshairScreen.x, CrosshairScreen.y), 8.f, IM_COL32(255, 255, 0, 255));
							globals::draw_list->AddCircleFilled(ImVec2(CrosshairScreen.x - TCircleOfst.x, CrosshairScreen.y - TCircleOfst.y), 2.f, IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddText(ImVec2(CrosshairScreen.x + 12.f, CrosshairScreen.y - 7.f), IM_COL32(255, 255, 255, 255), it.info.c_str());
							globals::draw_list->AddText(ImVec2(CrosshairScreen.x + 12.f, CrosshairScreen.y - -7.f), IM_COL32(255, 255, 255, 255), it.type.c_str());
							globals::draw_list->AddLine(ImVec2(cx, cy), ImVec2(CrosshairScreen.x, CrosshairScreen.y), IM_COL32(0, 0, 0, 255), 2.f);
							globals::draw_list->AddCircle(ImVec2(cx, cy), 10.f, utils::to_im32(Color::White), 255);
						}
					}
				}
            }
		}
	}

	void DrawRing3D()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::smoke_helper)
			return;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
			{
				for (auto& it : x.second)
				{
					Vector ppos = it.pos;

					auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsSmoke() && dist <= 500.f)
						{
							VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::White, 1.0f);
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

		Vector InfoPosScreen;

		for (auto& x : data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
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
							globals::draw_list->AddRectFilled(ImVec2(InfoPosScreen.x - 41.f, InfoPosScreen.y - 75.f), ImVec2(InfoPosScreen.x + size.x - 12.f, InfoPosScreen.y - 60.f), IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddText(ImVec2(InfoPosScreen.x - it.info.length() - 15.f, InfoPosScreen.y - 75.f), IM_COL32(255, 255, 255, 255), it.info.c_str());
						}
					}
				}
			}
		}
	}
}