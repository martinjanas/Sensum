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
        return reinterpret_cast<CSkeletonInstance*>(this);
    }

    void SetMeshGroupMask(uint64_t mask)
    {
        using fn = void(__fastcall*)(void*, uint64_t);
        static auto addr = modules::client.get_sig(xxh::get_hash("SetMeshGroupMask")).as<void*>();
        if (!addr)
            return;

        auto set_mesh_group_mask = reinterpret_cast<fn>(addr);
        set_mesh_group_mask(this, mask);
    }
};

