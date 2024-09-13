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

enum Collision_Group_t
{
    COLLISION_GROUP_NONE = 0,
    COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
    COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
    COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
    COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
    COLLISION_GROUP_PLAYER,
    COLLISION_GROUP_BREAKABLE_GLASS,
    COLLISION_GROUP_VEHICLE,
    COLLISION_GROUP_PLAYER_MOVEMENT, // For HL2, same as Collision_Group_Player, for TF2, this filters out other players and CBaseObjects
    COLLISION_GROUP_NPC,			// Generic NPC group
    COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
    COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
    COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
    COLLISION_GROUP_PROJECTILE,		// Projectiles!
    COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
    COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
    COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
    COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

    COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
    COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other

    LAST_SHARED_COLLISION_GROUP
};

class VPhysicsCollisionAttribute_t
{
    NETVAR(int, m_nInteractsAs, "VPhysicsCollisionAttribute_t", "m_nInteractsAs");
    NETVAR(int, m_nInteractsWith, "VPhysicsCollisionAttribute_t", "m_nInteractsWith");
    NETVAR(int, m_nInteractsExclude, "VPhysicsCollisionAttribute_t", "m_nInteractsExclude");
    NETVAR(int, m_nEntityId, "VPhysicsCollisionAttribute_t", "m_nEntityId");
    NETVAR(int, m_nOwnerId, "VPhysicsCollisionAttribute_t", "m_nOwnerId");
    NETVAR(int, m_nHierarchyId, "VPhysicsCollisionAttribute_t", "m_nHierarchyId");
    NETVAR(int, m_nCollisionGroup, "VPhysicsCollisionAttribute_t", "m_nCollisionGroup");
    NETVAR(int, m_nCollisionFunctionMask, "VPhysicsCollisionAttribute_t", "m_nCollisionFunctionMask");
};

class CCollisionProperty //TODO: Add more members later
{
public:
    //NETVAR(VPhysicsCollisionAttribute_t, m_collisionAttribute, "CCollisionProperty", "m_collisionAttribute");
    NETVAR(Vector, m_vecMins, "CCollisionProperty", "m_vecMins");
    NETVAR(Vector, m_vecMaxs, "CCollisionProperty", "m_vecMaxs");
    NETVAR(float, m_flBoundingRadius, "CCollisionProperty", "m_flBoundingRadius");
};

struct EmitSound_t
{
    int							m_nChannel;
    char const*                 m_pSoundName;
    float						m_flVolume;
    void*				        m_SoundLevel;
    int							m_nFlags;
    int							m_nPitch;
    const Vector* m_pOrigin;
    float						m_flSoundTime; ///< NOT DURATION, but rather, some absolute time in the future until which this sound should be delayed
    float*                      m_pflSoundDuration;
    bool						m_bEmitCloseCaption;
    bool						m_bWarnOnMissingCloseCaption;
    bool						m_bWarnOnDirectWaveReference;
    int							m_nSpeakerEntity;
    CUtlVector<Vector>	        m_UtlVecSoundOrigin;  ///< Actual sound origin(s) (can be multiple if sound routed through speaker entity(ies) )
    void*	                    m_hSoundScriptHash;
    int							m_nSoundEntryVersion;
};

class CHandle;
class CBaseEntity : public CEntityInstance
{
public:
    NETVAR(CHandle, m_hGroundEntity, "C_BaseEntity", "m_hGroundEntity");
	NETVAR(int, m_iHealth, "C_BaseEntity", "m_iHealth");
	NETVAR(LifeState_t, m_lifeState, "C_BaseEntity", "m_lifeState");
    NETVAR(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    NETVAR(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
    NETVAR(CCollisionProperty*, m_pCollision, "C_BaseEntity", "m_pCollision");
    NETVAR(CGlowProperty*, m_GlowProperty, "C_BaseModelEntity", "m_Glow");
    NETVAR(Vector, m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset");
    NETVAR(uint32_t, m_fFlags, "C_BaseEntity", "m_fFlags");

    //int EmitSound(EmitSound_t& params, const char* sound_name, float sound_time) //crashing/throwing exceptions
    //{
    //    using fn = int (__thiscall*)(void*, EmitSound_t&, const char*, float);

    //    static auto addr = modules::client.pattern_scanner.scan("E8 ? ? ? ? 41 2B FE", "EmitSound()").as();

    //    auto emit_sound = reinterpret_cast<fn>(addr);

    //    if (emit_sound)
    //        return emit_sound(this, params, sound_name, 0.0f);

    //    return -1;
    //}

    HitboxSet_t* GetHitboxSet(int i)
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

    int HitboxToWorldTransform(HitboxSet_t* hitbox_set, Transform_t* out_transform) //Crashing?
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
};





