#include "CBaseEntity.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CHandle.h"

CHandle CBaseEntity::GetHandle()
{
    CEntityIdentity* identity_entity = this->m_pEntity();

    if (identity_entity)
        return CHandle(identity_entity->GetEntityIndex(), identity_entity->GetSerialNumber() - (identity_entity->m_flags() & 1));

    return CHandle();
}

std::uint32_t CBaseEntity::GetOwnerHandle()
{
    std::uint32_t result = -1;

    if (this && m_pCollision() && !(m_pCollision()->m_usSolidFlags() & FSOLID_NOT_SOLID))
        result = this->m_hOwnerEntity().GetEntryIndex();

    return result;
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

HitboxSet_t* CBaseEntity::GetHitboxSet(int i)
{
    using fn = HitboxSet_t * (__thiscall*)(void*, int);

    static auto addr = modules::client.pattern_scanner.scan("E8 ?? ?? ?? ?? 48 8B F0 48 85 C0 0F 84 35 02 00 00", "GetHitboxSet()").add(0x1).abs().as();

    if (!addr)
        return nullptr;

    const auto get_hitbox_set = reinterpret_cast<fn>(addr);

    if (get_hitbox_set)
        return get_hitbox_set(this, i);

    return nullptr;
}

bool CBaseEntity::ComputeSurroundingBox(Vector* mins, Vector* maxs)
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

int CBaseEntity::HitboxToWorldTransform(HitboxSet_t* hitbox_set, Transform_t* out_transform) //Crashing?
{
    using fn = int(__thiscall*)(void*, HitboxSet_t*, Transform_t*, int max_studio_bones);

    static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 45 33 F6 4C 63 E0").add(0x1).abs().as();

    if (!addr)
        return -1;

    const auto hitbox_to_world_transform = reinterpret_cast<fn>(addr);

    if (hitbox_to_world_transform)
        return hitbox_to_world_transform(this, hitbox_set, out_transform, 256); //1024

    return -1;
}
