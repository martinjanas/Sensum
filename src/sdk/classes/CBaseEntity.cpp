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

HitboxSet_t* CBaseEntity::GetHitboxSet(int i)
{
    using fn = HitboxSet_t * (__thiscall*)(void*, int);
    static auto addr = modules::client.get_sig_addr(FNV("CBaseEntity::GetHitboxSet"), __FUNCTION__).as();
    if (!addr)
        return nullptr;

    const auto get_hitbox_set = reinterpret_cast<fn>(addr);
    if (get_hitbox_set)
        return get_hitbox_set(this, i);
}

int CBaseEntity::HitboxToWorldTransform(HitboxSet_t* hitbox_set, Transform_t* out_transform)
{
    using fn = int(__thiscall*)(void*, HitboxSet_t*, Transform_t*, int max_studio_bones); 
    static auto addr = modules::client.get_sig_addr(FNV("CBaseEntity::HitboxToWorldTransform"), __FUNCTION__).as();
    if (!addr)
        return -1;

    const auto hitbox_to_world_transform = reinterpret_cast<fn>(addr);
    if (hitbox_to_world_transform)
        return hitbox_to_world_transform(this, hitbox_set, out_transform, 1024);
}

int CBaseEntity::EmitSound(EmitSound_t& params, const char* sound_name, float sound_time)
{
    using fn = int(__thiscall*)(void*, EmitSound_t&, const char*, float);
    static auto addr = modules::client.get_sig_addr(FNV("CBaseEntity::EmitSound"), __FUNCTION__).as();
    if (!addr)
        return 0;

    auto emit_sound = reinterpret_cast<fn>(addr);
    if (emit_sound)
        return emit_sound(this, params, sound_name, 0.0f);
}
