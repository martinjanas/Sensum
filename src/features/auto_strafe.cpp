#include "features.h"

bool backward_style = false;

void features::auto_strafe(CUserCmd* cmd)
{
	if (cmd->buttons & IN_FORWARD)
		backward_style = false;
	else if (cmd->buttons & IN_BACK)
		backward_style = true;

	if (!g::local_player || !g::local_player->IsAlive())
		return;

	if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK || cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		return;

	if (cmd->mousedx <= 1 && cmd->mousedx >= -1)
		return;

	if (g::local_player->m_fFlags() & FL_ONGROUND)
		return;

	if (g::local_player->m_nMoveType() == MOVETYPE_NOCLIP)
		return;

	if (g::local_player->m_nMoveType() == MOVETYPE_LADDER)
		return;

	if (backward_style)
		cmd->sidemove = cmd->mousedx < 0.f ? 450.f : -450.f;
	else
		cmd->sidemove = cmd->mousedx < 0 ? -450.0f : 450.0f;
}