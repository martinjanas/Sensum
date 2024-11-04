#pragma once

#include "../../sdk/helpers/netvars.h"
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/classes/CBaseEntity.h"

class GameTick_t 
{
public:
    NETVAR(int32_t, m_Value, "GameTick_t", "m_Value");
};

class C_EconItemView
{
public:
    NETVAR(int, m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh"); //This probably outputs garbage data too
    NETVAR(int, m_iItemIDLow, "C_EconItemView", "m_iItemIDLow"); //This probably outputs garbage data too
    NETVAR(int, m_iAccountID, "C_EconItemView", "m_iAccountID"); //This probably outputs garbage data too
    NETVAR(uint16_t, m_iItemDefinitionIndex, "C_EconItemView", "m_iItemDefinitionIndex"); //This outputs garbage data
};

class C_AttributeContainer
{
public:
    NETVAR(C_EconItemView, m_Item, "C_AttributeContainer", "m_Item");
};

enum EItemDefinitionIndex : uint16_t
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_DUAL_BERETTAS = 2,
	WEAPON_FIVE_SEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A4 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP = 24,
	WEAPON_MP5SD = 23,
	WEAPON_XM1024 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCENDIARY = 48,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_M4A1_S = 60,
	WEAPON_USP_S = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525
};

enum CSWeaponType : std::uint32_t
{
    KNIFE,
    PISTOL,
    SMG,
    RIFLE,
    SHOTGUN,
    SNIPER,
    LMG,
    C4,
    TASER,
    GRENADE,
    EQUIPMENT,
    HEALTHSHOT
};

class CCSWeaponBaseVData
{
public:
    NETVAR(CSWeaponType, m_WeaponType, "CCSWeaponBaseVData", "m_WeaponType");
};

class CBasePlayerWeapon : public CBaseEntity
{
public:
    NETVAR(int, m_iClip1, "C_BasePlayerWeapon", "m_iClip1");
    NETVAR(int, m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit");
    NETVAR(C_AttributeContainer, m_AttributeManager, "C_EconEntity", "m_AttributeManager");
	NETVAR(GameTick_t, m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick")

	CCSWeaponBaseVData* GetWeaponData();

    uint16_t m_iItemDefinitionIndex()
    {
        return m_AttributeManager().m_Item().m_iItemDefinitionIndex();
    }
};

