#include "CBaseEntity.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CHandle.h"
#include "../../sdk/helpers/console.h"

CHandle CBaseEntity::GetHandle()
{
    CEntityIdentity* identity_entity = this->m_pEntity();

    if (identity_entity)
        return CHandle(identity_entity->GetEntityIndex(), identity_entity->GetSerialNumber() - (identity_entity->m_flags() & 1));

    return CHandle();
}

std::uint32_t CBaseEntity::GetOwnerHandle()
{
    std::uint32_t entry_index = -1;

    if (this && m_pCollision() && !(m_pCollision()->m_usSolidFlags() & FSOLID_NOT_SOLID))
        entry_index = this->m_hOwnerEntity().GetEntryIndex();

    return entry_index;
}

CHandle CBaseEntity::m_hGroundEntity()
{
    constexpr auto hash = fnv::hash_constexpr("C_BaseEntity->m_hGroundEntity");
    static auto offset = netvars::get_offset_by_hash_cached(hash);
    auto handle = *reinterpret_cast<CHandle*>(reinterpret_cast<uintptr_t>(this) + offset);
    
    return handle;
}

CHandle CBaseEntity::m_hOwnerEntity()
{
    constexpr auto hash = fnv::hash_constexpr("C_BaseEntity->m_hOwnerEntity");
    static auto offset = netvars::get_offset_by_hash_cached(hash);
    auto handle = *reinterpret_cast<CHandle*>(reinterpret_cast<uintptr_t>(this) + offset);

    return handle;
}

CHandle CBaseEntity::m_hOldMoveParent()
{
    constexpr auto hash = fnv::hash_constexpr("C_BaseEntity->m_hOldMoveParent");
    static auto offset = netvars::get_offset_by_hash_cached(hash);
    auto handle = *reinterpret_cast<CHandle*>(reinterpret_cast<uintptr_t>(this) + offset);

    return handle;
}

HitboxSet_t* CBaseEntity::GetHitboxSet(int i)
{
    using fn = HitboxSet_t * (__thiscall*)(void*, int);
    static auto addr = modules::client.scan("48 89 5C 24 08 48 89 74 24 10 57 48 81 EC 40 01 00 00 8B DA 48 8B F9 E8 ?? ?? ?? ??", "GetHitboxSet").as();
    if (!addr)
        return nullptr;

    const auto get_hitbox_set = reinterpret_cast<fn>(addr);
    if (get_hitbox_set)
        return get_hitbox_set(this, i);

    return nullptr;
}

int CBaseEntity::HitboxToWorldTransform(HitboxSet_t* hitbox_set, Transform_t* out_transform)
{
    using fn = int(__thiscall*)(void*, HitboxSet_t*, Transform_t*, int max_studio_bones); 
    static auto addr = modules::client.scan("48 89 5C 24 ? 55 57 41 54 41 56 41 57 48 83 EC 20", "HitboxToWorldTransform").as();
    if (!addr)
        return -1;

    const auto hitbox_to_world_transform = reinterpret_cast<fn>(addr);
    if (hitbox_to_world_transform)
        return hitbox_to_world_transform(this, hitbox_set, out_transform, 1024);

    return -1;
}
