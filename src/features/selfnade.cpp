#include "features.h"

void features::selfnade(CUserCmd* cmd)
{
	if (!g::local_player)
		return;

	if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
		return;

	if (!g::local_player->IsAlive())
		return;

	const auto& active_wpn = g::local_player->m_hActiveWeapon();
	
	if (!active_wpn)
		return;

	if (!active_wpn->IsGrenade())
		return;

	if (active_wpn->m_flThrowStrength() >= 0.11f || active_wpn->m_flThrowStrength() <= 0.10f || cmd->viewangles.pitch > -88.0f)
		return;

	cmd->buttons &= ~IN_ATTACK;
	cmd->buttons &= ~IN_ATTACK2;
}