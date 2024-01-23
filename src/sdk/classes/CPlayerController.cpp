#include "CPlayerController.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/helpers/fnv.h"

bool CBasePlayerController::InAir()
{
	return m_hGroundEntity() == nullptr;
}

bool CBasePlayerController::IsAlive()
{
	return m_iHealth() > 0 || m_lifeState() == LIFE_ALIVE;
}