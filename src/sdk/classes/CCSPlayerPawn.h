#pragma once
#include "CBaseEntity.h"
#include "CPlayerWeaponServices.h"
#include "CCSPlayerController.h"
#include "CPlayerCameraServices.h"

class CCSPlayerPawn : public CBaseEntity
{
public:
    NETVAR(int, m_iShotsFired, "C_CSPlayerPawnBase", "m_iShotsFired");
    NETVAR(float, m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration");
    NETVAR(int, m_iIDEntIndex, "C_CSPlayerPawnBase", "m_iIDEntIndex");
    NETVAR(Vector, m_pGlowServices, "C_CSPlayerPawn", "m_pGlowServices");
    NETVAR(Vector, m_aimPunchAngle, "C_CSPlayerPawn", "m_aimPunchAngle"); //I dont think this is Vector
    NETVAR(CPlayer_CameraServices*, m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices");
    NETVAR(Vector, m_vOldOrigin, "C_BasePlayerPawn", "m_vOldOrigin"); //Returns 0 0 0
    NETVAR(CPlayerWeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
    NETVAR(Vector, m_angEyeAngles, "C_CSPlayerPawnBase", "m_angEyeAngles");

    Vector GetEyePos();
    bool InAir();
    bool IsAlive();
};