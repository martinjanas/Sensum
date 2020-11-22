#include "features.h"
#include "../settings/globals.h"
#include "../helpers/console.h"
#include "../helpers/input.h"

namespace silent_walk
{
	bool is_enabled(CUserCmd* cmd)
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return false;

		if (!settings::misc::silent_walk)
			return false;

		auto weapon_handle = g::local_player->m_hActiveWeapon();
		if (!weapon_handle)
			return false;

		return true;
	}

	void handle(CUserCmd* cmd)
	{
		if (!is_enabled(cmd))
			return;

		Vector moveDir = Vector(0.f, 0.f, 0.f);
		float maxSpeed = 130.f;
		int movetype = g::local_player->m_nMoveType();
		bool InAir = !(g::local_player->m_fFlags() & FL_ONGROUND);
		if (movetype == MOVETYPE_FLY || movetype == MOVETYPE_NOCLIP || InAir || cmd->buttons & IN_DUCK || !(cmd->buttons & IN_SPEED))
			return;
		moveDir.x = cmd->sidemove;
		moveDir.y = cmd->forwardmove;
		moveDir = math::ClampMagnitude(moveDir, maxSpeed);
		cmd->sidemove = moveDir.x;
		cmd->forwardmove = moveDir.y;
		if (!(g::local_player->m_vecVelocity().Length2D() > maxSpeed + 1))
			cmd->buttons &= ~IN_SPEED;
	}
}