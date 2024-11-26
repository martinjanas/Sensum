#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/classes/GameTick_t.h"
#include "../../sdk/classes/CHandle.h"

//C_PlantedC4 : C_BaseModelEntity : C_BaseEntity
class CPlantedC4
{
public:
	NETVAR(bool, m_bBombTicking, "C_PlantedC4", "m_bBombTicking")
	NETVAR(int, m_nBombSite, "C_PlantedC4", "m_nBombSite")
	NETVAR(float, m_flC4Blow, "C_PlantedC4", "m_flC4Blow")
	NETVAR(bool, m_bHasExploded, "C_PlantedC4", "m_bHasExploded")
	NETVAR(float, m_flTimerLength, "C_PlantedC4", "m_flTimerLength")
	NETVAR(bool, m_bBeingDefused, "C_PlantedC4", "m_bBeingDefused")
	NETVAR(bool, m_bC4Activated, "C_PlantedC4", "m_bC4Activated")
	NETVAR(float, m_flDefuseLength, "C_PlantedC4", "m_flDefuseLength")
	NETVAR(float, m_flDefuseCountDown, "C_PlantedC4", "m_flDefuseCountDown")
	NETVAR(bool, m_bBombDefused, "C_PlantedC4", "m_bBombDefused")
	NETVAR(CHandle, m_hBombDefuser, "C_PlantedC4", "m_hBombDefuser")
	NETVAR(CHandle, m_pBombDefuser, "C_PlantedC4", "m_pBombDefuser")
	NETVAR(float, m_fLastDefuseTime, "C_PlantedC4", "m_fLastDefuseTime")
};