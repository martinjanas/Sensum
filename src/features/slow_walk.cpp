#include "features.h"
#include "../settings/globals.h"
#include "../helpers/console.h"
#include "../helpers/input.h"

namespace slow_walk
{
	bool is_enabled(CUserCmd* cmd)
	{
		if (!g::local_player || !g::local_player->IsAlive())
			return false;

		if (globals::binds::slow_walk == 0 || !input_system::is_key_down(globals::binds::slow_walk))
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

		const float amount = 0.0034f * 60.f;
		const auto velocity = g::local_player->m_vecVelocity();

		QAngle direction;
		math::vector2angles(velocity, direction);

		float speed = velocity.Length2D();
		direction.yaw = cmd->viewangles.yaw - direction.yaw;

		Vector forward;
		math::angle2vectors(direction, forward);

		Vector source = forward * -speed;

		auto weapon_data = uintptr_t(g::local_player->m_hActiveWeapon()->GetWeaponData());
		const auto max_weapon_speed = /**reinterpret_cast<float*>(weapon_data + 0x0134)*/ 250.f;
		if (speed >= max_weapon_speed * amount)
		{
			cmd->forwardmove = source.x;
			cmd->sidemove = source.y;
		}
	}
}