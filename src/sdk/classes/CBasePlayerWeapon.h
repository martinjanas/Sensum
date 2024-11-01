#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"

class GameTick_t 
{
public:
    NETVAR(int32_t, m_Value, "GameTick_t", "m_Value");
};

class C_EconItemView
{
public:
    NETVAR(int, m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh"); //This probably outputs garbage data too
    NETVAR(int, m_iItemIDLow, "C_EconItemView", "m_iItemIDLow"); //This probably outputs garbage data too
    NETVAR(int, m_iAccountID, "C_EconItemView", "m_iAccountID"); //This probably outputs garbage data too
    NETVAR(uint16_t, m_iItemDefinitionIndex, "C_EconItemView", "m_iItemDefinitionIndex"); //This outputs garbage data
};

class C_AttributeContainer
{
public:
    NETVAR(C_EconItemView, m_Item, "C_AttributeContainer", "m_Item");
};

class CBasePlayerWeapon
{
public:
    NETVAR(int, m_iClip1, "C_BasePlayerWeapon", "m_iClip1");
    NETVAR(int, m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit");
    NETVAR(C_AttributeContainer, m_AttributeManager, "C_EconEntity", "m_AttributeManager");
    NETVAR(GameTick_t, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")

    uint16_t m_iItemDefinitionIndex()
    {
        return m_AttributeManager().m_Item().m_iItemDefinitionIndex();
    }
};

