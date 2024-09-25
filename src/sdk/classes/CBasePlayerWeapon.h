#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"

class C_EconItemView
{
public:
    NETVAR(int, m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh");
    NETVAR(int, m_iItemIDLow, "C_EconItemView", "m_iItemIDLow");
    NETVAR(int, m_iAccountID, "C_EconItemView", "m_iAccountID");
};

class C_AttributeContainer
{
public:
    NETVAR(C_EconItemView*, m_Item, "C_AttributeContainer", "m_Item");
};

class CBasePlayerWeapon
{
public:
    NETVAR(int, m_iClip1, "C_BasePlayerWeapon", "m_iClip1");
    NETVAR(int, m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit");
    NETVAR(C_AttributeContainer*, m_AttributeManager, "C_EconEntity", "m_AttributeManager");
};

