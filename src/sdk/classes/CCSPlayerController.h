#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CPlayerMoneyServices.h"
#include "../../sdk/classes/CHandle.h"

//C_CSPlayerPawn: C_CSPlayerPawnBase: C_BasePlayerPawn
//CCSPlayerController : CBasePlayerController: C_BaseEntity

class CCSPlayerController : public CBaseEntity
{
public:
    NETVAR(CCSPlayerController_InGameMoneyServices*, m_pInGameMoneyServices, "CCSPlayerController", "m_pInGameMoneyServices");
    NETVAR(const char*, m_sSanitizedPlayerName, "CCSPlayerController", "m_sSanitizedPlayerName");
    NETVAR(bool, m_bIsLocalPlayerController, "CBasePlayerController", "m_bIsLocalPlayerController");
    NETVAR(const char*, m_iszPlayerName, "CBasePlayerController", "m_iszPlayerName");
    NETVAR(CHandle, m_hPawn, "CBasePlayerController", "m_hPawn");
    NETVAR(CHandle, m_hPlayerPawn, "CCSPlayerController", "m_hPlayerPawn");
};