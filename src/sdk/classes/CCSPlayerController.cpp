#include "CCSPlayerController.h"
#include "../../sdk/classes/CBaseEntity.h"

Vector CCSPlayerController::GetEyePos()
{
    return m_pGameSceneNode()->m_vecOrigin() + m_vecViewOffset();
}