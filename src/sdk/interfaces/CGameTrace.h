#pragma once

#include "../../sdk/math/Vector.h"
#include "../../sdk/math/QAngle.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CCSPlayerPawn.h"

enum BuiltInInteractionLayer_t
{
	LAYER_INDEX_CONTENTS_SOLID = 0,
	LAYER_INDEX_CONTENTS_HITBOX,
	LAYER_INDEX_CONTENTS_TRIGGER,
	LAYER_INDEX_CONTENTS_SKY,
	LAYER_INDEX_FIRST_USER,
	LAYER_INDEX_NOT_FOUND = -1,
	LAYER_INDEX_MAX_ALLOWED = 64,
};

enum StandardInteractionLayers_t
{
	LAYER_INDEX_CONTENTS_PLAYER_CLIP = LAYER_INDEX_FIRST_USER,
	LAYER_INDEX_CONTENTS_NPC_CLIP,
	LAYER_INDEX_CONTENTS_BLOCK_LOS,
	LAYER_INDEX_CONTENTS_BLOCK_LIGHT,
	LAYER_INDEX_CONTENTS_LADDER,
	LAYER_INDEX_CONTENTS_PICKUP,
	LAYER_INDEX_CONTENTS_BLOCK_SOUND,
	LAYER_INDEX_CONTENTS_NODRAW,
	LAYER_INDEX_CONTENTS_WINDOW,
	LAYER_INDEX_CONTENTS_PASS_BULLETS,
	LAYER_INDEX_CONTENTS_WORLD_GEOMETRY,
	LAYER_INDEX_CONTENTS_WATER,
	LAYER_INDEX_CONTENTS_SLIME,
	LAYER_INDEX_CONTENTS_TOUCH_ALL,
	LAYER_INDEX_CONTENTS_PLAYER,
	LAYER_INDEX_CONTENTS_NPC,
	LAYER_INDEX_CONTENTS_DEBRIS,
	LAYER_INDEX_CONTENTS_PHYSICS_PROP,
	LAYER_INDEX_CONTENTS_NAV_IGNORE,
	LAYER_INDEX_CONTENTS_NAV_LOCAL_IGNORE,
	LAYER_INDEX_CONTENTS_POST_PROCESSING_VOLUME,
	LAYER_INDEX_CONTENTS_UNUSED_LAYER3,
	LAYER_INDEX_CONTENTS_CARRIED_OBJECT,
	LAYER_INDEX_CONTENTS_PUSHAWAY,
	LAYER_INDEX_CONTENTS_SERVER_ENTITY_ON_CLIENT,
	LAYER_INDEX_CONTENTS_CARRIED_WEAPON,
	LAYER_INDEX_CONTENTS_STATIC_LEVEL,
	LAYER_INDEX_FIRST_MOD_SPECIFIC,
};

enum ModSpecificInteractionLayers_t
{
	LAYER_INDEX_CONTENTS_CSGO_TEAM1 = LAYER_INDEX_FIRST_MOD_SPECIFIC,
	LAYER_INDEX_CONTENTS_CSGO_TEAM2,
	LAYER_INDEX_CONTENTS_CSGO_GRENADE_CLIP,
	LAYER_INDEX_CONTENTS_CSGO_DRONE_CLIP,
	LAYER_INDEX_CONTENTS_CSGO_MOVEABLE,
	LAYER_INDEX_CONTENTS_CSGO_OPAQUE,
	LAYER_INDEX_CONTENTS_CSGO_MONSTER,
	LAYER_INDEX_CONTENTS_CSGO_UNUSED_LAYER,
	LAYER_INDEX_CONTENTS_CSGO_THROWN_GRENADE,
};

enum BuiltInCollisionGroup_t
{
	// Default layer, always collides with everything.
	COLLISION_GROUP_ALWAYS = 0, // "always"
	// This is how you turn off all collisions for an object - move it to this group
	COLLISION_GROUP_NONPHYSICAL, // "never" 
	// Trigger layer, never collides with anything, only triggers/interacts.  Use when querying for interaction layers.
	COLLISION_GROUP_TRIGGER, // "trigger"
	// Conditionally solid means that the collision response will be zero or as defined in the table when there are matching interactions
	COLLISION_GROUP_CONDITIONALLY_SOLID, // needs interactions
	// First unreserved collision layer index.
	COLLISION_GROUP_FIRST_USER,
	// Hard limit of 254 due to memory layout, and these are never visible to scene queries.
	COLLISION_GROUPS_MAX_ALLOWED = 64,
};

enum StandardCollisionGroups_t
{
	COLLISION_GROUP_DEFAULT = COLLISION_GROUP_FIRST_USER,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world, static stuff and triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
	// TF2, this filters out other players and CBaseObjects
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
	COLLISION_GROUP_NPC_SCRIPTED,	// Used for NPCs in scripts that should not collide with each other
	COLLISION_GROUP_PZ_CLIP,
	COLLISION_GROUP_PROPS,

	LAST_SHARED_COLLISION_GROUP,
};

#define	CONTENTS_EMPTY						0ull // No contents

#define CONTENTS_SOLID						( 1ull << LAYER_INDEX_CONTENTS_SOLID )
#define CONTENTS_HITBOX						( 1ull << LAYER_INDEX_CONTENTS_HITBOX )
#define CONTENTS_TRIGGER					( 1ull << LAYER_INDEX_CONTENTS_TRIGGER )
#define CONTENTS_SKY						( 1ull << LAYER_INDEX_CONTENTS_SKY )

#define CONTENTS_PLAYER_CLIP				( 1ull << LAYER_INDEX_CONTENTS_PLAYER_CLIP )
#define CONTENTS_NPC_CLIP					( 1ull << LAYER_INDEX_CONTENTS_NPC_CLIP )
#define CONTENTS_BLOCK_LOS					( 1ull << LAYER_INDEX_CONTENTS_BLOCK_LOS )
#define CONTENTS_BLOCK_LIGHT				( 1ull << LAYER_INDEX_CONTENTS_BLOCK_LIGHT )
#define CONTENTS_LADDER						( 1ull << LAYER_INDEX_CONTENTS_LADDER )
#define CONTENTS_PICKUP						( 1ull << LAYER_INDEX_CONTENTS_PICKUP )
#define CONTENTS_BLOCK_SOUND				( 1ull << LAYER_INDEX_CONTENTS_BLOCK_SOUND )
#define CONTENTS_NODRAW						( 1ull << LAYER_INDEX_CONTENTS_NODRAW )
#define CONTENTS_WINDOW						( 1ull << LAYER_INDEX_CONTENTS_WINDOW )
#define CONTENTS_PASS_BULLETS				( 1ull << LAYER_INDEX_CONTENTS_PASS_BULLETS )
#define CONTENTS_WORLD_GEOMETRY				( 1ull << LAYER_INDEX_CONTENTS_WORLD_GEOMETRY )
#define CONTENTS_WATER						( 1ull << LAYER_INDEX_CONTENTS_WATER )
#define CONTENTS_SLIME						( 1ull << LAYER_INDEX_CONTENTS_SLIME )
#define CONTENTS_TOUCH_ALL					( 1ull << LAYER_INDEX_CONTENTS_TOUCH_ALL )
#define CONTENTS_PLAYER						( 1ull << LAYER_INDEX_CONTENTS_PLAYER )
#define CONTENTS_NPC						( 1ull << LAYER_INDEX_CONTENTS_NPC )
#define CONTENTS_DEBRIS						( 1ull << LAYER_INDEX_CONTENTS_DEBRIS )
#define CONTENTS_PHYSICS_PROP				( 1ull << LAYER_INDEX_CONTENTS_PHYSICS_PROP )
#define CONTENTS_NAV_IGNORE					( 1ull << LAYER_INDEX_CONTENTS_NAV_IGNORE )
#define CONTENTS_NAV_LOCAL_IGNORE			( 1ull << LAYER_INDEX_CONTENTS_NAV_LOCAL_IGNORE )
#define CONTENTS_POST_PROCESSING_VOLUME		( 1ull << LAYER_INDEX_CONTENTS_POST_PROCESSING_VOLUME )
#define CONTENTS_UNUSED_LAYER3				( 1ull << LAYER_INDEX_CONTENTS_UNUSED_LAYER3 )
#define CONTENTS_CARRIED_OBJECT				( 1ull << LAYER_INDEX_CONTENTS_CARRIED_OBJECT )
#define CONTENTS_PUSHAWAY					( 1ull << LAYER_INDEX_CONTENTS_PUSHAWAY )
#define CONTENTS_SERVER_ENTITY_ON_CLIENT	( 1ull << LAYER_INDEX_CONTENTS_SERVER_ENTITY_ON_CLIENT )
#define CONTENTS_CARRIED_WEAPON				( 1ull << LAYER_INDEX_CONTENTS_CARRIED_WEAPON )
#define CONTENTS_STATIC_LEVEL				( 1ull << LAYER_INDEX_CONTENTS_STATIC_LEVEL )

#define CONTENTS_CSGO_TEAM1					( 1ull << LAYER_INDEX_CONTENTS_CSGO_TEAM1 )
#define CONTENTS_CSGO_TEAM2					( 1ull << LAYER_INDEX_CONTENTS_CSGO_TEAM2 )
#define CONTENTS_CSGO_GRENADE_CLIP			( 1ull << LAYER_INDEX_CONTENTS_CSGO_GRENADE_CLIP )
#define CONTENTS_CSGO_DRONE_CLIP			( 1ull << LAYER_INDEX_CONTENTS_CSGO_DRONE_CLIP )
#define CONTENTS_CSGO_MOVEABLE				( 1ull << LAYER_INDEX_CONTENTS_CSGO_MOVEABLE )
#define CONTENTS_CSGO_OPAQUE				( 1ull << LAYER_INDEX_CONTENTS_CSGO_OPAQUE )
#define CONTENTS_CSGO_MONSTER				( 1ull << LAYER_INDEX_CONTENTS_CSGO_MONSTER )
#define CONTENTS_CSGO_UNUSED_LAYER			( 1ull << LAYER_INDEX_CONTENTS_CSGO_UNUSED_LAYER )
#define CONTENTS_CSGO_THROWN_GRENADE		( 1ull << LAYER_INDEX_CONTENTS_CSGO_THROWN_GRENADE )



// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define	MASK_ALL					(~0ull)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_PLAYER|CONTENTS_NPC|CONTENTS_PASS_BULLETS)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_PLAYER_CLIP|CONTENTS_WINDOW|CONTENTS_PLAYER|CONTENTS_NPC|CONTENTS_PASS_BULLETS)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_NPC_CLIP|CONTENTS_WINDOW|CONTENTS_PLAYER|CONTENTS_NPC|CONTENTS_PASS_BULLETS)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_NPC_CLIP|CONTENTS_WINDOW|CONTENTS_PLAYER|CONTENTS_NPC)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_SLIME)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_PLAYER|CONTENTS_NPC|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_PLAYER|CONTENTS_NPC|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_PASS_BULLETS)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_PLAYER|CONTENTS_NPC)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_PASS_BULLETS)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_PLAYER_CLIP|CONTENTS_PASS_BULLETS)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_NPC_CLIP|CONTENTS_PASS_BULLETS)

//Custom defined masks
#define MASK_PLAYER_VISIBLE (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_PLAYER | CONTENTS_NPC | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_BLOCK_LOS)


enum RayType_t : uint8_t
{
	RAY_TYPE_LINE = 0,
	RAY_TYPE_SPHERE,
	RAY_TYPE_HULL,
	RAY_TYPE_CAPSULE,
	RAY_TYPE_MESH,
};

struct Ray_t
{
	Ray_t() { Init(Vector(0.0f, 0.0f, 0.0f)); }
	Ray_t(const Vector& vStartOffset) { Init(vStartOffset); }
	Ray_t(const Vector& vCenter, float flRadius) { Init(vCenter, flRadius); }
	Ray_t(const Vector& vMins, const Vector& vMaxs) { Init(vMins, vMaxs); }
	Ray_t(const Vector& vCenterA, const Vector& vCenterB, float flRadius) { Init(vCenterA, vCenterB, flRadius); }
	Ray_t(const Vector& vMins, const Vector& vMaxs, const Vector* pVertices, int nNumVertices) { Init(vMins, vMaxs, pVertices, nNumVertices); }

	void Init(const Vector& vStartOffset)
	{
		m_Line.m_vStartOffset = vStartOffset;
		m_Line.m_flRadius = 0.0f;
		m_eType = RAY_TYPE_LINE;
	}

	void Init(const Vector& vCenter, float flRadius)
	{
		if (flRadius > 0.0f)
		{
			m_Sphere.m_vCenter = vCenter;
			m_Sphere.m_flRadius = flRadius;
			m_eType = RAY_TYPE_SPHERE;
		}
		else
		{
			Init(vCenter);
		}
	}

	void Init(const Vector& vMins, const Vector& vMaxs)
	{
		if (vMins != vMaxs)
		{
			m_Hull.m_vMins = vMins;
			m_Hull.m_vMaxs = vMaxs;
			m_eType = RAY_TYPE_HULL;
		}
		else
		{
			Init(vMins);
		}
	}

	void Init(const Vector& vCenterA, const Vector& vCenterB, float flRadius)
	{
		if (vCenterA != vCenterB)
		{
			if (flRadius > 0.0f)
			{
				m_Capsule.m_vCenter[0] = vCenterA;
				m_Capsule.m_vCenter[1] = vCenterB;
				m_Capsule.m_flRadius = flRadius;
				m_eType = RAY_TYPE_CAPSULE;
			}
			else
			{
				Init(vCenterA, vCenterB);
			}
		}
		else
		{
			Init(vCenterA, flRadius);
		}
	}

	void Init(const Vector& vMins, const Vector& vMaxs, const Vector* pVertices, int nNumVertices)
	{
		m_Mesh.m_vMins = vMins;
		m_Mesh.m_vMaxs = vMaxs;
		m_Mesh.m_pVertices = pVertices;
		m_Mesh.m_nNumVertices = nNumVertices;
		m_eType = RAY_TYPE_MESH;
	}

	struct Line_t
	{
		Vector m_vStartOffset;
		float m_flRadius;
	};

	struct Sphere_t
	{
		Vector m_vCenter;
		float m_flRadius;
	};

	struct Hull_t
	{
		Vector m_vMins;
		Vector m_vMaxs;
	};

	struct Capsule_t
	{
		Vector m_vCenter[2];
		float m_flRadius;
	};

	struct Mesh_t
	{
		Vector m_vMins;
		Vector m_vMaxs;
		const Vector* m_pVertices;
		int m_nNumVertices;
	};

	union
	{
		Line_t 		m_Line;
		Sphere_t 	m_Sphere;
		Hull_t 		m_Hull;
		Capsule_t 	m_Capsule;
		Mesh_t 		m_Mesh;
	};

	RayType_t m_eType;
};

struct TraceHitboxData_t
{
	std::byte pad01[0x38];
	int m_nHitGroup;
	std::byte pad02[0x4];
	int m_nHitboxId;
};

struct SurfaceData_t
{
	std::byte pad01[0x8];
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	std::byte pad02[0x4];
	int m_iMaterial;
};

struct Trace_t
{
	Trace_t() = default;

	SurfaceData_t* GetSurfaceData();

	int GetHitboxID()
	{
		if (m_pHitboxData)
			return m_pHitboxData->m_nHitboxId;
		return -1;
	}

	int GetHitgroup()
	{
		if (m_pHitboxData)
			return m_pHitboxData->m_nHitGroup;
		return -1;
	}

	//void* m_pSurface;					// 0x0
	//CBaseEntity* m_pHitEntity;			// 0x8
	//TraceHitboxData_t* m_pHitboxData;	// 0x10
	//std::byte pad01[0x38];				// 0x18
	//uint32_t m_uContents;				// 0x50
	//std::byte pad02[0x24];				// 0x54
	//Vector m_vecStartPos;				// 0x78
	//Vector m_vecEndPos;					// 0x84
	//Vector m_vecNormal;					// 0x90
	//Vector m_vecPosition;				// 0x9C
	//std::byte pad03[0x4];				// 0xA8
	//float m_flFraction;					// 0xAC
	//std::byte pad04[0x6];				// 0xB0
	//bool m_bAllSolid;					// 0xB6
	//bool m_bStartSolid;					// 0xB7
	//bool m_bSomeBool;					// 0xB8 

	void* m_pSurface;
	CBaseEntity* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	std::byte pad01[0x38];
	std::uint32_t m_uContents;
	std::byte pad02[0x24];
	Vector m_vecStartPos;
	Vector m_vecEndPos;
	Vector m_vecNormal;
	Vector m_vecPosition;
	std::byte pad03[0x4];
	float m_flFraction;
	std::byte pad04[0x6];
	bool m_bAllSolid;
	std::byte pad05[0x4D];
};
static_assert(sizeof(Trace_t) == 264);

enum PhysicsTraceType_t
{
	VPHYSICS_TRACE_EVERYTHING = 0,
	VPHYSICS_TRACE_STATIC_ONLY,
	VPHYSICS_TRACE_MOVING_ONLY,
	VPHYSICS_TRACE_TRIGGERS_ONLY,
	VPHYSICS_TRACE_STATIC_AND_MOVING,
};

struct TraceFilter_t
{
	std::byte pad01[0x8];							// 0x00
	std::int64_t m_uTraceMask;						// 0x08
	std::array<std::int64_t, 2> m_v1;				// 0x10
	std::array<std::int32_t, 4> m_arrSkipHandles;   // 0x20
	std::array<std::int16_t, 2> m_arrCollisions;    // 0x30
	std::int16_t m_v2;								// 0x34
	std::uint8_t m_nLayer;								// 0x36
	std::uint8_t m_v4;								// 0x37 //flags
	std::uint8_t m_flags;								// 0x38

	TraceFilter_t() = default;
	TraceFilter_t(std::uint32_t uMask, CBaseEntity* localplayer, CBaseEntity* player, int nLayer);

	static TraceFilter_t* InitEntitiesOnly(TraceFilter_t* thisptr, CBaseEntity* skip, uint32_t mask, int layer)
	{
		using fn = TraceFilter_t*(__thiscall*)(void*, CBaseEntity*, uint32_t, int, int16_t);
		static auto addr = modules::client.get_sig_addr(FNV("TraceFilter_t::InitEntitiesOnly", __FUNCTION__)).as();
		if (!addr)
			return nullptr;

		auto init_entities_only = reinterpret_cast<fn>(addr);
		if (init_entities_only)
			return init_entities_only(thisptr, skip, mask, layer, 15);
	}
};

class CGameTrace
{
public:
	bool TraceShape(Ray_t* ray, const Vector& start, const Vector& end, TraceFilter_t* filter, Trace_t* trace)
	{
		using fn = bool(__fastcall*)(void*, Ray_t*, const Vector&, const Vector&, TraceFilter_t*, Trace_t*);
		static auto addr = modules::client.get_sig_addr(FNV("CGameTrace::TraceShape", __FUNCTION__)).as();
		if (!addr)
			return false;

		auto trace_shape = reinterpret_cast<fn>(addr);
		if (trace_shape)
			return trace_shape(this, ray, start, end, filter, trace);
	}

	bool ClipRayToEntity(Ray_t* ray, const Vector& start, const Vector& end, CCSPlayerPawn* skip, TraceFilter_t* filter, Trace_t* trace)
	{
		using fn = bool(__fastcall*)(void*, Ray_t*, const Vector&, const Vector&, CCSPlayerPawn*, TraceFilter_t*, Trace_t*);
		static auto addr = modules::client.get_sig_addr(FNV("CGameTrace::ClipRayToEntity", __FUNCTION__)).as();
		if (!addr)
			return false;

		auto clip_ray_to_entity = reinterpret_cast<fn>(addr);
		if (clip_ray_to_entity)
			return clip_ray_to_entity(this, ray, start, end, skip, filter, trace);
	}

	//TraceRay - index 56
	/*void TraceRay(const Ray_t& ray, unsigned int fMask, TraceFilter_t* pTraceFilter, Trace_t* pTrace)
	{
		GetVirtual<void(__thiscall*)(void*, const Ray_t&, uint32_t mask, TraceFilter_t*, Trace_t*)>(this, 56)(this, ray, fMask, pTraceFilter, pTrace);
		VTable::GetThiscall<void>(this, 56, ray, fMask, pTraceFilter, pTrace);
	}*/
};