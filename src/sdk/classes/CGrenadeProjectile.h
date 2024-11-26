#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/classes/CBaseEntity.h"

class CGrenade : public CBaseEntity
{
public:
	NETVAR(float, m_flDetonateTime, "C_BaseGrenade", "m_flDetonateTime")
	NETVAR(float, m_flDamage, "C_BaseGrenade", "m_flDamage")
};

class CGrenadeProjectile : public CGrenade
{
public:

};