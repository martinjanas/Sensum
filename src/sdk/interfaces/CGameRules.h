#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"

//Actual type: C_CSGameRules
class CGameRules
{
public:
	NETVAR(bool, m_bFreezePeriod, "C_CSGameRules", "m_bFreezePeriod");
	NETVAR(bool, m_bWarmupPeriod, "C_CSGameRules", "m_bWarmupPeriod");
	NETVAR(bool, m_bBombPlanted, "C_CSGameRules", "m_bBombPlanted");
	NETVAR(bool, m_bBombDropped, "C_CSGameRules", "m_bBombDropped");
};