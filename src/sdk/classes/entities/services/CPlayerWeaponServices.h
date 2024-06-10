#pragma once
#include "../../../../helpers/netvars.h"
#include "../../../../helpers/vfunc.h"
#include "../../../source_engine/CHandle.h"
#include "../../../../sdk/classes/entities/players/CBasePlayerWeapon.h"


class CPlayerWeaponServices
{
public:
    NETVAR(CHandle, m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon");
};

