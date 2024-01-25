#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CGameSceneNode.h"
#include "../../sdk/classes/CEntityInstance.h"
#include "../../sdk/classes/CPlayerGlowProperty.h"

enum LifeState_t : uint8_t
{
    LIFE_ALIVE,
    LIFE_DYING,
    LIFE_DEAD,
    LIFE_RESPAWNABLE,
    LIFE_RESPAWNING
};

//class entity_instance {
//public:
//    NETVAR(m_pEntity, "CEntityInstance", "m_pEntity", entity_identity*);
//
//    template<typename T> requires std::derived_from<T, entity_instance>
//    VIRTUAL_FUNCTION(get_ref_handle, void, 2, (this, handle), handle<T>* handle)
//
//        template<typename T> requires std::derived_from<T, entity_instance>
//    auto get_ref_handle() noexcept {
//        handle<T> handle;
//        get_ref_handle(&handle);
//        return handle;
//    }
//
//};

class CBaseAnimating
{
public:

    HitboxSet_t* GetHitboxSet(int i)
    {
        using fn = HitboxSet_t*(__thiscall*)(void*, int);

        static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 44 8D 48 07").add(0x1).abs().as();

        if (!addr)
            return nullptr;

        const auto get_hitbox_set = reinterpret_cast<fn>(addr);

        if (get_hitbox_set)
            return reinterpret_cast<HitboxSet_t*>(get_hitbox_set(this, i));
    }

    /*uint32_t HitboxToWorldTransform(const HitboxSet_t* hitbox_set, mat3x4_t hitbox_to_world, int size)
    {
        using fn = uint32_t(__thiscall*)(void*, const HitboxSet_t*, mat3x4_t, int);
        static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 33 F6 4C 63 E0").add(0x1).abs().as();

        const auto hitbox_to_world_transform = reinterpret_cast<fn>(addr);

        if (hitbox_to_world_transform)
            return hitbox_to_world_transform(this, hitbox_set, hitbox_to_world, size);
    }*/
};

class CBaseEntity;
class CCollisionProperty //TODO: Add more members later
{
public:
    std::byte pad001[sizeof(uintptr_t)];
    CBaseEntity* outer;

    NETVAR(Vector, m_vecMins, "CCollisionProperty", "m_vecMins");
    NETVAR(Vector, m_vecMaxs, "CCollisionProperty", "m_vecMaxs");
};

class CBaseEntity : public CEntityInstance
{
public:
    NETVAR(void*, m_hGroundEntity, "C_BaseEntity", "m_hGroundEntity");
	NETVAR(int, m_iHealth, "C_BaseEntity", "m_iHealth");
	NETVAR(LifeState_t, m_lifeState, "C_BaseEntity", "m_lifeState");
    NETVAR(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    NETVAR(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
    NETVAR(CCollisionProperty*, m_pCollision, "C_BaseEntity", "m_pCollision");
    NETVAR(CGlowProperty*, m_GlowProperty, "C_BaseModelEntity", "m_Glow");
    NETVAR(Vector, m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset");

    CBaseAnimating* GetBaseAnimating()
    {
        return GetVirtual<CBaseAnimating*(__thiscall*)(void*)>(this, 44)(this);
    }

    bool ComputeSurroundingBox(Vector* mins, Vector* maxs)
    {
        using fn = bool(__thiscall*)(void*, Vector*, Vector*);

        static auto addr = modules::client.pattern_scanner.scan("E9 ? ? ? ? F6 43 5B FD").add(0x1).abs().as();

        if (!addr)
            return false;

        const auto compute_surrounding_box = reinterpret_cast<fn>(addr);

        if (compute_surrounding_box)
            return compute_surrounding_box(this, mins, maxs);

        return false;
    }
};




