#include "features.h"
#include "../settings/globals.h"

void features::thirdperson()
{
	if (!g::local_player)
		return;

	g::input->m_fCameraInThirdPerson = globals::binds::thirdperson::enabled && g::local_player->IsAlive();
	if (!g::input->m_fCameraInThirdPerson)
		return;

	auto& weapon = g::local_player->m_hActiveWeapon();

	const auto weapon_type = weapon->GetWeaponData()->WeaponType;

	if (weapon_type == WEAPONTYPE_GRENADE)
		g::input->m_fCameraInThirdPerson = false;

	if ((weapon_type == WEAPONTYPE_PISTOL || weapon_type == WEAPONTYPE_MACHINEGUN || weapon_type == WEAPONTYPE_RIFLE || weapon_type == WEAPONTYPE_SHOTGUN || weapon_type == WEAPONTYPE_SNIPER_RIFLE || weapon_type == WEAPONTYPE_SUBMACHINEGUN) && settings::misc::disable_on_weapon)
		g::input->m_fCameraInThirdPerson = false;

	QAngle angles;
	g::engine_client->GetViewAngles(angles);

	QAngle backward(angles.pitch, angles.yaw + 180.f, angles.roll);
	backward.NormalizeClamp();

	Vector range;
	math::angle2vectors(backward, range);
	range *= 8192.f;

	const auto start = g::local_player->GetEyePos();

	CTraceFilter filter;
	filter.pSkip = g::local_player;

	Ray_t ray;
	ray.Init(start, start + range);

	trace_t tr;
	g::engine_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	angles.roll = std::min<int>(start.DistTo(tr.endpos), 150); // 150 is better distance

	g::input->m_vecCameraOffset = angles;

	g::local_player->UpdateClientSideAnimation();
}