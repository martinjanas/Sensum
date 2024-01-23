#include "../settings/settings.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.h"

#include <format>

namespace features
{
	void fast_stop(CUserCmd* cmd)
	{
		if (!settings::misc::fast_stop)
			return;

		if (!g::local_player)
			return;

		if (g::local_player->IsDead())
			return;

		if (g::local_player->m_nMoveType() == MOVETYPE_NOCLIP || g::local_player->m_nMoveType() == MOVETYPE_LADDER ||
			g::local_player->InAir() || cmd->buttons & IN_JUMP)
			return;

		if (cmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
			return;

		const Vector& velocity = g::local_player->m_vecVelocity();
		const float speed = velocity.Length2D();
		
		if (speed <= 15.f)	
			return;

		QAngle direction;
		math::vector2angles(velocity, direction);
		direction.yaw -= cmd->viewangles.yaw;

		Vector final_dir;
		math::angle2vectors(direction * -speed, final_dir);

		final_dir.x = std::clamp(final_dir.x, -450.0f, 450.0f);
		final_dir.y = std::clamp(final_dir.y, -450.0f, 450.0f);

		cmd->forwardmove = final_dir.x;
		cmd->sidemove = final_dir.y;
	}
}