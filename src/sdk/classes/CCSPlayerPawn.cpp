#include "CCSPlayerPawn.h"
#include "../../sdk/classes/CHandle.h"

Vector CCSPlayerPawn::GetEyePos()
{
    return m_pGameSceneNode()->m_vecOrigin() + m_vecViewOffset();
}

bool CCSPlayerPawn::InAir()
{	
	return m_hGroundEntity().Get() == nullptr;
}

bool CCSPlayerPawn::IsAlive()
{
	return m_iHealth() > 0 || m_lifeState() == LIFE_ALIVE;
}