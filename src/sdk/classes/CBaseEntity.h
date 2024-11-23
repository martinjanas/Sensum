#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CGameSceneNode.h"
#include "../../sdk/classes/CEntityInstance.h"
#include "../../sdk/classes/CPlayerGlowProperty.h"
#include "../../sdk/classes/CHandle.h"

enum LifeState_t : uint8_t
{
    LIFE_ALIVE,
    LIFE_DYING,
    LIFE_DEAD,
    LIFE_RESPAWNABLE,
    LIFE_RESPAWNING
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

enum SolidType_t
{
    SOLID_NONE = 0,    // no solid model
    SOLID_BSP = 1,    // a BSP tree
    SOLID_BBOX = 2,    // an AABB
    SOLID_OBB = 3,    // an OBB (not implemented yet)
    SOLID_OBB_YAW = 4,    // an OBB, constrained so that it can only yaw
    SOLID_CUSTOM = 5,    // Always call into the entity for tests
    SOLID_VPHYSICS = 6,    // solid vphysics object, get vcollide from the model and collide with that
    SOLID_LAST,
};

enum SolidFlags_t
{
    FSOLID_CUSTOMRAYTEST = 0x0001,    // Ignore solid type + always call into the entity for ray tests
    FSOLID_CUSTOMBOXTEST = 0x0002,    // Ignore solid type + always call into the entity for swept box tests
    FSOLID_NOT_SOLID = 0x0004,    // Are we currently not solid?
    FSOLID_TRIGGER = 0x0008,    // This is something may be collideable but fires touch functions
    // even when it's not collideable (when the FSOLID_NOT_SOLID flag is set)
    FSOLID_NOT_STANDABLE = 0x0010,    // You can't stand on this
    FSOLID_VOLUME_CONTENTS = 0x0020,    // Contains volumetric contents (like water)
    FSOLID_FORCE_WORLD_ALIGNED = 0x0040,    // Forces the collision rep to be world-aligned even if it's SOLID_BSP or SOLID_VPHYSICS
    FSOLID_USE_TRIGGER_BOUNDS = 0x0080,    // Uses a special trigger bounds separate from the normal OBB
    FSOLID_ROOT_PARENT_ALIGNED = 0x0100,    // Collisions are defined in root parent's local coordinate space
    FSOLID_TRIGGER_TOUCH_DEBRIS = 0x0200,    // This trigger will touch debris objects

    FSOLID_MAX_BITS = 10
};

class CCollisionProperty //TODO: Add more members later
{
public:
    //NETVAR(VPhysicsCollisionAttribute_t, m_collisionAttribute, "CCollisionProperty", "m_collisionAttribute");
    NETVAR(Vector, m_vecMins, "CCollisionProperty", "m_vecMins");
    NETVAR(Vector, m_vecMaxs, "CCollisionProperty", "m_vecMaxs");
    NETVAR(float, m_flBoundingRadius, "CCollisionProperty", "m_flBoundingRadius");
    NETVAR(uint8_t, m_usSolidFlags, "CCollisionProperty", "m_usSolidFlags");

    uint16_t CollisionMask()
    {
        return *reinterpret_cast<uint16_t*>(this + 0x38);
    }
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

class CBaseEntity : public CEntityInstance
{
public:
	NETVAR(int, m_iHealth, "C_BaseEntity", "m_iHealth");
	NETVAR(LifeState_t, m_lifeState, "C_BaseEntity", "m_lifeState");
    NETVAR(uint8_t, m_iTeamNum, "C_BaseEntity", "m_iTeamNum");
    NETVAR(CGameSceneNode*, m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode");
    NETVAR(CCollisionProperty*, m_pCollision, "C_BaseEntity", "m_pCollision");
    NETVAR(CGlowProperty*, m_GlowProperty, "C_BaseModelEntity", "m_Glow");
    NETVAR(Vector, m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset");
    NETVAR(uint32_t, m_fFlags, "C_BaseEntity", "m_fFlags");   
    NETVAR(uint32_t, m_nSubclassID, "C_BaseEntity", "m_nSubclassID");

    CHandle m_hGroundEntity();
    CHandle m_hOwnerEntity();

    CHandle GetHandle();
    std::uint32_t GetOwnerHandle();
    HitboxSet_t* GetHitboxSet(int i);
    int HitboxToWorldTransform(HitboxSet_t* hitbox_set, Transform_t* out_transform);
    int EmitSound(EmitSound_t& params, const char* sound_name, float sound_time);
};





