#pragma once

#include "../../sdk/math/Vector.h"
#include "../../sdk/math/QAngle.h"
#include "../../sdk/classes/CBaseEntity.h"
#include "../../sdk/classes/CCSPlayerPawn.h"

#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	0x80

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
#define CONTENTS_UNUSED6		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace


// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox



// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)


struct Ray_t
{
	Vector start;   // 0x00
	Vector end;     // 0x0C
	Vector mins;    // 0x18
	Vector maxs;    // 0x24
	//some bool at 0x28 - probably something related to m_bAllSolid
	std::byte pad01[0x4]; // 0x30
	uint8_t unknown;  // 0x34
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
		return 0;
	}

	int GetHitgroup()
	{
		if (m_pHitboxData)
			return m_pHitboxData->m_nHitGroup;
		return 0;
	}

	void* m_pSurface;					// 0x0
	CBaseEntity* m_pHitEntity;			// 0x8
	TraceHitboxData_t* m_pHitboxData;	// 0x10
	std::byte pad01[0x38];				// 0x18
	uint32_t m_uContents;				// 0x50
	std::byte pad02[0x24];				// 0x54
	Vector m_vecStartPos;				// 0x78
	Vector m_vecEndPos;					// 0x84
	Vector m_vecNormal;					// 0x90
	Vector m_vecPosition;				// 0x9C
	std::byte pad03[0x4];				// 0xA8
	float m_flFraction;					// 0xAC
	std::byte pad04[0x6];				// 0xB0
	bool m_bAllSolid;					// 0xB6
	bool m_bStartSolid;					// 0xB7
	bool m_bSomeBool;					// 0xB8 
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

	TraceFilter_t(std::uint32_t mask, CBaseEntity* skip_entity, int layer)
	{
		using fn = TraceFilter_t * (__fastcall*)(void*, CBaseEntity* skip_entity, uint32_t mask, int layer, int16_t flags);
		static auto addr = modules::client.scan("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33", "TraceFilter_t::InitEntitiesOnly").as();

		auto init_entities_only = reinterpret_cast<fn>(addr);
		if (init_entities_only)
			init_entities_only(this, skip_entity, mask, layer, 7); //15 = CollisionMask
	}

	TraceFilter_t* InitEntitiesOnly(CBaseEntity* skip_entity, uint32_t mask, int layer)
	{
		using fn = TraceFilter_t*(__fastcall*)(void*, CBaseEntity* skip_entity, uint32_t mask, int layer, int16_t flags);
		static auto addr = modules::client.scan("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33", "TraceFilter_t::InitEntitiesOnly").as();

		auto init_entities_only = reinterpret_cast<fn>(addr);
		if (init_entities_only)
			return init_entities_only(this, skip_entity, mask, layer, 15); //15 = CollisionMask
	}
};

class CGameTrace
{
public:
	bool TraceShape(Ray_t* ray, const Vector& start, const Vector& end, TraceFilter_t* filter, Trace_t* trace)
	{
		using fn = bool(__fastcall*)(void*, Ray_t*, const Vector&, const Vector&, TraceFilter_t*, Trace_t*); //fastcall
		//E8 ? ? ? ? 80 7D 57 00 + 0x1 abs
		static const auto& addr = modules::client.scan("48 89 5C 24 20 48 89 4C 24 08 55 56 41", "TraceShape").as();

		auto trace_shape = reinterpret_cast<fn>(addr);
		if (trace_shape)
			return trace_shape(this, ray, start, end, filter, trace);

		return false;
	}

	bool ClipRayToEntity(Ray_t* ray, const Vector& start, const Vector& end, CCSPlayerPawn* pawn, TraceFilter_t* filter, Trace_t* trace)
	{
		using fn = bool(__fastcall*)(void*, Ray_t*, const Vector&, const Vector&, CCSPlayerPawn*, TraceFilter_t*, Trace_t*);
		static const auto addr = modules::client.scan("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C", "ClipRayToEntity").as();

		auto clip_ray_to_entity = reinterpret_cast<fn>(addr);
		if (clip_ray_to_entity)
			return clip_ray_to_entity(this, ray, start, end, pawn, filter, trace);

		return false;
	}
};