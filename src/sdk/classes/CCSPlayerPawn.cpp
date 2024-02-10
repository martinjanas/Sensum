#include "CCSPlayerPawn.h"

Vector CCSPlayerPawn::GetEyePos()
{
    return m_pGameSceneNode()->m_vecOrigin() + m_vecViewOffset();
}

bool CCSPlayerPawn::InAir()
{
	return m_hGroundEntity() == nullptr;
}

bool CCSPlayerPawn::IsAlive()
{
	return m_iHealth() > 0 || m_lifeState() == LIFE_ALIVE;
}