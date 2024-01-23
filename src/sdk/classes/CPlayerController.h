#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CPlayerWeaponServices.h"
#include "../../sdk/classes/CPlayerMoneyServices.h"

class CBaseEntity;

class CBasePlayerController : public CBaseEntity
{
public:

    NETVAR(const char*, m_iszPlayerName, "CBasePlayerController", "m_iszPlayerName");
    NETVAR(CHandle, m_hPawn, "CBasePlayerController", "m_hPawn");

    bool InAir();
    bool IsAlive();
};


class CCSPlayerController : public CBasePlayerController
{
public:

    NETVAR(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices, "CCSPlayerController", "m_pInGameMoneyServices");
    NETVAR(const char*, m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");
    NETVAR(bool, m_bIsLocalPlayerController, "CBasePlayerController", "m_bIsLocalPlayerController");
};
