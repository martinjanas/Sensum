#pragma once
#include "CBaseEntity.h"
#include "CPlayerWeaponServices.h"
#include "CCSPlayerController.h"
#include "CPlayerCameraServices.h"

class CCSPlayerPawn : public CBaseEntity
{
public:
    NETVAR(int, m_iShotsFired, "C_CSPlayerPawn", "m_iShotsFired");
    NETVAR(float, m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration");
    NETVAR(int, m_iIDEntIndex, "C_CSPlayerPawnBase", "m_iIDEntIndex");
    NETVAR(Vector, m_pGlowServices, "C_CSPlayerPawn", "m_pGlowServices");
    NETVAR(QAngle, m_aimPunchAngle, "C_CSPlayerPawn", "m_aimPunchAngle");
    NETVAR(CPlayer_CameraServices*, m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices");
    NETVAR(Vector, m_vOldOrigin, "C_BasePlayerPawn", "m_vOldOrigin"); //Returns 0 0 0
    NETVAR(CPlayerWeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
    NETVAR(QAngle, m_angEyeAngles, "C_CSPlayerPawnBase", "m_angEyeAngles");
    NETVAR(QAngle, v_angle, "C_BasePlayerPawn", "v_angle");
    NETVAR(CUtlVector<QAngle>, m_aimPunchCache, "C_CSPlayerPawn", "m_aimPunchCache");
    NETVAR(bool, m_bIsScoped, "C_CSPlayerPawn", "m_bIsScoped");
    NETVAR(bool, m_bGunGameImmunity, "C_CSPlayerPawnBase", "m_bGunGameImmunity");

    Vector GetEyePos();
    bool InAir();
    bool IsAlive();
};
