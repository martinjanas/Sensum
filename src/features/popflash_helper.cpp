#include "popflash_helper.h"
#include "../render/render.h"

std::map<std::string, std::vector<popflash_data_t>> popflash_data =
{
	{ "mirage", popflash_data_mirage },
	{ "inferno", popflash_data_inferno },
	{ "dust2", popflash_data_dust2 },
	{ "nuke", popflash_data_nuke },
	{ "overpass", popflash_data_overpass },
	{ "train", popflash_data_train },
	{ "cache", popflash_data_cache },
	{ "cbble", popflash_data_cobblestone }
};

namespace visuals
{
	void PopflashHelperAimbot(CUserCmd* cmd)
	{
		if (!g::local_player)
			return;

		for (auto& x : popflash_data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
			{
				for (auto& it : x.second)
				{
					if (g::local_player->m_hActiveWeapon())
					{
						auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);
						if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 70)
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

	void RenderCirclePopflash()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::flash_helper)
			return;

		if (g::engine_client->IsConsoleVisible())
			return;

		Vector InfoPosScreen;
		Vector CrosshairScreen;

		int x, y;
		g::engine_client->GetScreenSize(x, y);

		int cy = y / 2;
		int cx = x / 2;

		for (auto& x : popflash_data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
			{
				for (auto& it : x.second)
				{
					Vector ppos = it.pos;
					Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
					Vector InfoPos = it.pos;
					Vector InfoAng = it.ang;
					Vector TCircleOfst = utils::CalcHelpPos(it.pos);

					auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);
					auto size = ImGui::CalcTextSize(it.info.c_str());
					auto _size = ImGui::CalcTextSize(it.type.c_str());

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 70 && math::world2screen(Crosshair, CrosshairScreen))
						{
							globals::draw_list->AddRectFilled(ImVec2(CrosshairScreen.x - 20.f, CrosshairScreen.y - 10.f), ImVec2(CrosshairScreen.x + size.x + 25.f, CrosshairScreen.y + 12.f), IM_COL32(0, 0, 0, 255));
							globals::draw_list->AddRectFilled(ImVec2(CrosshairScreen.x + 8.f, CrosshairScreen.y - -10.f), ImVec2(CrosshairScreen.x + _size.x + 15.f, CrosshairScreen.y + 22.f), IM_COL32(0, 0, 0, 255));
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

	void drawring_3d_popflash()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::flash_helper)
			return;

		if (g::engine_client->IsConsoleVisible())
			return;

		for (auto& x : popflash_data)
		{
			if (strstr(g::engine_client->GetLevelName(), x.first.c_str()))
			{
				for (auto& it : x.second)
				{
					Vector ppos = it.pos;
					Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
					Vector InfoPos = it.pos;
					Vector InfoAng = it.ang;

					auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

					if (g::local_player->m_hActiveWeapon())
					{
						if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 1000.f)
						{
							VGSHelper::Get().drawring_3d(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
						}
					}
				}
			}
		}
	}
}