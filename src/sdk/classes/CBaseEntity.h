#pragma once
#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CGameSceneNode.h"
#include "../../sdk/classes/CEntityInstance.h"
#include "../../sdk/classes/CPlayerGlowProperty.h"

constexpr std::ptrdiff_t m_Glow = 0xB98; // CGlowProperty

enum LifeState_t : uint8_t
{
    LIFE_ALIVE,
    LIFE_DYING,
    LIFE_DEAD,
    LIFE_RESPAWNABLE,
    LIFE_RESPAWNING
};

class CCollisionProperty //TODO: Add more members later
{
public:
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

    void ComputeSurroundingBox(Vector* mins, Vector* maxs)
    {
        using fn = void(__fastcall*)(void*, Vector*, Vector*);

        auto addr = modules::client.pattern_scanner.scan("E9 ? ? ? ? F6 43 5B FD").abs().as();

        const auto compute_surrounding_box = reinterpret_cast<fn>(addr);

        if (compute_surrounding_box)
            compute_surrounding_box(this, mins, maxs);
    }
};


