#include "popflash_helper.h"

namespace visuals
{
	void PopflashHelperAimbot(CUserCmd* cmd)
	{
		if (!g::local_player)
			return;

		if (strstr(g::engine_client->GetLevelName(), "mirage"))
		{
			for (auto& it : Mirage)
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

		if (strstr(g::engine_client->GetLevelName(), "inferno"))
		{
			for (auto& it : Inferno)
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

		if (strstr(g::engine_client->GetLevelName(), "dust2"))
		{
			for (auto& it : Dust2)
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

		if (strstr(g::engine_client->GetLevelName(), "nuke"))
		{
			for (auto& it : Nuke)
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

		if (strstr(g::engine_client->GetLevelName(), "overpass"))
		{
			for (auto& it : Overpass)
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

		if (strstr(g::engine_client->GetLevelName(), "train"))
		{
			for (auto& it : Train)
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

		if (strstr(g::engine_client->GetLevelName(), "cache"))
		{
			for (auto& it : Cache)
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

		if (strstr(g::engine_client->GetLevelName(), "cbble"))
		{
			for (auto& it : Cobblestone)
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

	void RenderCirclePopflash()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::flash_helper)
			return;

		Vector InfoPosScreen;
		Vector CrosshairScreen;

		int x, y;
		g::engine_client->GetScreenSize(x, y);

		int cy = y / 2;
		int cx = x / 2;

		if (strstr(g::engine_client->GetLevelName(), "mirage"))
		{
			for (auto& it : Mirage)
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

		if (strstr(g::engine_client->GetLevelName(), "inferno"))
		{
			for (auto& it : Inferno)
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

		if (strstr(g::engine_client->GetLevelName(), "dust2"))
		{
			for (auto& it : Dust2)
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

		if (strstr(g::engine_client->GetLevelName(), "nuke"))
		{
			for (auto& it : Nuke)
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

		if (strstr(g::engine_client->GetLevelName(), "overpass"))
		{
			for (auto& it : Overpass)
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

		if (strstr(g::engine_client->GetLevelName(), "train"))
		{
			for (auto& it : Train)
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

		if (strstr(g::engine_client->GetLevelName(), "cache"))
		{
			for (auto& it : Cache)
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

		if (strstr(g::engine_client->GetLevelName(), "cbble"))
		{
			for (auto& it : Cobblestone)
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

	void DrawRing3DPopflash()
	{
		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected() || !g::local_player)
			return;

		if (!settings::misc::flash_helper)
			return;

		if (strstr(g::engine_client->GetLevelName(), "mirage"))
		{
			for (auto& it : Mirage)
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
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "inferno"))
		{
			for (auto& it : Inferno)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "dust2"))
		{
			for (auto& it : Dust2)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "nuke"))
		{
			for (auto& it : Nuke)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "overpass"))
		{
			for (auto& it : Overpass)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "train"))
		{
			for (auto& it : Train)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "cache"))
		{
			for (auto& it : Cache)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}

		if (strstr(g::engine_client->GetLevelName(), "cbble"))
		{
			for (auto& it : Cobblestone)
			{
				Vector ppos = it.pos;
				Vector Crosshair = g::local_player->GetEyePos() + (utils::CalcDir(it.ang) * 250.f);
				Vector InfoPos = it.pos;
				Vector InfoAng = it.ang;

				auto dist = g::local_player->m_vecOrigin().DistTo(it.pos);

				if (g::local_player->m_hActiveWeapon())
				{
					if (g::local_player->m_hActiveWeapon()->IsFlash() && dist <= 500.f)
					{
						VGSHelper::Get().DrawRing3D(ppos.x, ppos.y, ppos.z - it.ring_height, 15.f, 255, Color::Red, 1.0f);
					}
				}
			}
		}
	}
}