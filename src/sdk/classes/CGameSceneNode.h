#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CSkeletonInstance.h"
#include "../../sdk/math/Vector.h"
#include "../../sdk/math/Transform.h"
#include "../../sdk/math/QAngle.h"

class CGameSceneNode
{
public:
    NETVAR(Vector, m_vecOrigin, "CGameSceneNode", "m_vecOrigin");
    NETVAR(Vector, m_vecAbsOrigin, "CGameSceneNode", "m_vecAbsOrigin");
    NETVAR(Transform_t, m_nodeToWorld, "CGameSceneNode", "m_nodeToWorld");
    NETVAR(QAngle, m_angRotation, "CGameSceneNode", "m_angRotation");

    CSkeletonInstance* GetSkeletonInstance()
    {
        return VTable::GetThiscall<CSkeletonInstance*>(this, 8);
    }
};

