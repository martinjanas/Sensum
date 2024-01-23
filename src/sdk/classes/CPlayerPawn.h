#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CPlayerWeaponServices.h"

class C_PostProcessingVolume
{
public:
    /*activePostProcessingVolume : setSchemaInt("C_PostProcessingVolume", "m_bExposureControl", 1)
        activePostProcessingVolume : setSchemaFloat("C_PostProcessingVolume", "m_flMinExposure", 0.15)
        activePostProcessingVolume : setSchemaFloat("C_PostProcessingVolume", "m_flMaxExposure", 0.15)*/

        NETVAR(int, m_bExposureControl, "C_PostProcessingVolume", "m_bExposureControl");
        NETVAR(float, m_flMinExposure, "C_PostProcessingVolume", "m_flMinExposure");
        NETVAR(float, m_flMaxExposure, "C_PostProcessingVolume", "m_flMaxExposure");
};

class CPlayer_CameraServices
{
public:

    NETVAR(C_PostProcessingVolume*, m_PostProcessingVolumes, "CPlayer_CameraServices", "m_PostProcessingVolumes");
};

//MJ: No, what is this?! How am I supposed to know which class to use? IM GONNA REBUILD THIS LATER!
//MJ: Implement CCSPlayerController & CBasePlayerPawn, no _Base

class CBasePlayerPawn : public CBaseEntity
{
public:

  
    NETVAR(CPlayer_CameraServices*, m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices");
    NETVAR(Vector, m_vOldOrigin, "C_BasePlayerPawn", "m_vOldOrigin");

    NETVAR(CPlayerWeaponServices*, m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices");
};

class CCSPlayerPawnBase : public CBasePlayerPawn
{
public:

    NETVAR(int, m_iShotsFired, "C_CSPlayerPawnBase", "m_iShotsFired");
    NETVAR(float, m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration");
    NETVAR(int, m_iIDEntIndex, "C_CSPlayerPawnBase", "m_iIDEntIndex");

};
class CCSPlayerPawn : public CCSPlayerPawnBase
{
public:

    NETVAR(Vector, m_pGlowServices, "C_CSPlayerPawn", "m_pGlowServices");
    NETVAR(Vector, m_aimPunchAngle, "C_CSPlayerPawn", "m_aimPunchAngle");

};



