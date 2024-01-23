#include "features.h"

auto jumped_last_tick = false;
auto should_fake_jump = false;

void features::bhop(CUserCmd* cmd)
{
	if (!g::local_player || !g::local_player->IsAlive())
		return;

	if (g::local_player->m_nMoveType() == MOVETYPE_LADDER)
		return;

	if (!jumped_last_tick && should_fake_jump)
	{
		should_fake_jump = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (g::local_player->m_fFlags() & FL_ONGROUND)
		{
			jumped_last_tick = true;
			should_fake_jump = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			jumped_last_tick = false;
		}
	}
	else
	{
		jumped_last_tick = false;
		should_fake_jump = false;
	}
}