#pragma once

#include "sdk.hpp"
#include "../helpers/utils.h"

#include <array>
#include <random>

#define MAX_PLAYERS 65
#define MAX_PATCHES 5
#define MAX_WEAPONS 64

class CAnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void* m_pOwner; //0x0030
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

class CAnimState
{
public:
	void* vtable;
	bool m_bFirstRunSinceInit;
	bool m_bFirstFootPlantSinceInit;
	char pad[2];
	int m_nUnknownInt;
	float m_flEyePositionSmoothLerp;
	float m_flStrafeChangeWeightSmoothFalloff;
	void* m_StandWalkAim;
	void* m_StandRunAim;
	void* m_CrouchWalkAim;
	int m_nUnknownArray[3];
	int m_nModelIndex;
	void* m_pPlayer;
	void* m_pActiveWeapon;
	void* m_pLastActiveWeapon;
	float m_flLastClientSideAnimationUpdateTime;
	int m_nLastClientSideAnimationUpdateFramecount;
	float m_flLastClientSideAnimationUpdateTimeDelta;
	float m_flEyeYaw;
	float m_flEyePitch;
	float m_flMoveYaw;
	float m_flCurrentFeetYaw;
	float m_flMoveYawIdeal;
	float m_flMoveYawCurrentToIdeal;
	float m_flFeetVelDirDelta;
	float pad_0049;
	float m_flFeetCycle;
	float m_flFeetWeight;
	float m_flUnknown2;
	float m_flDuckAmount;
	float m_flHitGroundCycle;
	float m_flMoveWeight;
	Vector m_vecOrigin;
	Vector m_vecLastOrigin;
	Vector m_vecVelocity;
	Vector m_vecVelocityNormalized;
	Vector m_vecLastAcceleratingVelocity;
	float m_flSpeed;
	float m_flAbsVelocityZ;
	float m_flSpeedAsPortionOfRunTopSpeed;
	float m_flRunningSpeed;
	float m_flDuckingSpeed;
	float m_flTimeSinceStartedMoving;
	float m_flTimeSinceStoppedMoving;
	bool m_bOnGround;
	bool m_bLanding;
	char pad_010A[2];
	float m_flNextLowerBodyYawUpdateTime;
	float m_flDurationInAir;
	float m_flLeftGroundHeight;
	float m_flHitGroundWeight;
	float m_flGroundFraction;
	bool m_bLandedOnGroundThisFrame;
	bool m_bLeftTheGroundThisFrame;
	char pad_0120[2];
	float m_flInAirSmoothValue;
	bool m_bOnLadder;
	char pad_0128[3];
	float m_flLandAnimMultiplier;
	float m_flWalkToRunTransition;
	bool m_bNotRunning;
	char pad_0135[3];
	bool m_bInBalanceAdjust;
	char pad_0141[3];
	void* m_ActivityModifiers;
	float m_flLadderWeight;
	int m_flLadderSpeed;
	float m_flTimeOfLastInjury;
	float m_flLastSetupLeanCurtime;
	Vector m_vecLastSetupLeanVelocity;
	Vector m_vecSetupLeanVelocityDelta;
	Vector m_vecSetupLeanVelocityInterpolated;
	float m_flLeanWeight;
	Vector m_vecTargetAcceleration;
	char pad_qzdqzdzqd[3];
	float m_flStrafeWeight;
	Vector m_vecAcceleration;
	float m_flStrafeCycle;
	float m_flStrafeChangeWeight;
	bool m_bStrafing;
	char m_bStrafingPad[3];
	float m_flStrafeChangeCycle;
	int m_nStrafeSequence;
	bool m_bUnknownBool__;
	bool m_bIsAccelerating;
}; // Size: 0x344

class c_base_entity : public IClientEntity
{
public:
	NETVAR(int, m_nModelIndex, "CBaseEntity", "m_nModelIndex");
	NETVAR(int, m_iTeamNum, "CBaseEntity", "m_iTeamNum");
	NETVAR(Vector, m_vecOrigin, "CBaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecAngles, "CBaseEntity", "m_vecAngles");
	NETVAR(bool, m_bShouldGlow, "CDynamicProp", "m_bShouldGlow");
	NETVAR(CHandle<c_base_player>, m_hOwnerEntity, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(bool, m_bSpotted, "CBaseEntity", "m_bSpotted");
	NETPROP(m_bSpotted_hook, "CBaseEntity", "m_bSpotted"); //EDIT
	NETVAR(bool, m_bSpottedByMask, "CBaseEntity", "m_bSpottedByMask");
	NETVAR(float, m_flAnimTime, "CBaseEntity", "m_flAnimTime");
	NETVAR_OFFSET(matrix3x4_t&, m_CollisionGroup, "CBaseEntity", "m_CollisionGroup", -0x30);
	NETVAR_OFFSET(int, GetIndex, "CBaseEntity", "m_bIsAutoaimTarget", +0x4);
		
	NETVAR(Vector, m_vecMins, "CBaseEntity", "m_vecMins");
	NETVAR(Vector, m_vecMaxs, "CBaseEntity", "m_vecMaxs");

	bool IsPlayer();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
	Vector& GetAbsOrigin();
	void SetAbsOrigin(const Vector& origin);
	const matrix3x4_t& m_rgflCoordinateFrame();
	void UpdateVisibilityAllEntities();
	int GetSequenceActivity(studiohdr_t* hdr, const int& sequence);

	static __forceinline c_base_entity* GetEntityByIndex(int index)
	{
		return static_cast<c_base_entity*>(g::entity_list->GetClientEntity(index));
	}

	template <typename A>
	static __forceinline A* GetEntityFromHandle(CBaseHandle h)
	{
		return static_cast<A*>(g::entity_list->GetClientEntityFromHandle(h));
	}

	static __forceinline c_base_entity* GetEntityFromHandle(CBaseHandle h)
	{
		return GetEntityFromHandle<c_base_entity>(h);
	}
};

class c_base_planted_c4 : public c_base_entity
{
public:
	NETVAR(bool, m_bBombTicking, "CPlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "CPlantedC4", "m_bBombDefused");
	NETVAR(float, m_flC4Blow, "CPlantedC4", "m_flC4Blow");
	NETVAR(float, m_flTimerLength, "CPlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "CPlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "CPlantedC4", "m_flDefuseCountDown");
	NETVAR(CHandle<c_base_player>, m_hBombDefuser, "CPlantedC4", "m_hBombDefuser");
	NETVAR(int, m_nBombSite, "CPlantedC4", "m_nBombSite");
};

class c_base_grenade : public c_base_entity
{
public:
	NETVAR(float, m_flDamage, "CBaseGrenade", "m_flDamage");
	NETVAR(int, m_DmgRadius, "CBaseGrenade", "m_DmgRadius");
	NETVAR(bool, m_bIsLive, "CBaseGrenade", "m_bIsLive");
	NETVAR(CHandle<c_base_player>, m_hThrower, "CBaseGrenade", "m_hThrower");
	NETVAR(Vector, m_vecVelocity, "CBaseGrenade", "m_vecVelocity");
	NETVAR(int, m_fFlags, "CBaseGrenade", "m_fFlags");

};

class c_base_attributable_item : public c_base_entity
{
private:
	using str_32 = char[32];
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "CBaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int, m_OriginalOwnerXuidLow, "CBaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int, m_OriginalOwnerXuidHigh, "CBaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int, m_nFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int, m_nFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int, m_nFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, m_flFallbackWear, "CBaseAttributableItem", "m_flFallbackWear");

	NETVAR(int, m_bInitialized, "CBaseAttributableItem", "m_bInitialized");
	NETVAR(short, m_iItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int, m_iEntityLevel, "CBaseAttributableItem", "m_iEntityLevel");
	NETVAR(int, m_iAccountID, "CBaseAttributableItem", "m_iAccountID");
	NETVAR(int, m_iItemIDLow, "CBaseAttributableItem", "m_iItemIDLow");
	NETVAR(int, m_iItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int, m_iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality");
	NETVAR(str_32, m_iCustomName, "CBaseAttributableItem", "m_szCustomName");

	void SetModelIndex(int modelIndex);
};

class c_base_weapon_world_model : public c_base_entity
{
public:
	NETVAR(int, m_nModelIndex, "CBaseWeaponWorldModel", "m_nModelIndex");
};

class c_base_combat_weapon : public c_base_attributable_item
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int, m_iClip1, "CBaseCombatWeapon", "m_iClip1");
	NETVAR(int, m_iPrimaryReserveAmmoCount, "CBaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
	NETVAR(int, m_iClip2, "CBaseCombatWeapon", "m_iClip2");
	NETVAR(float_t, m_flRecoilIndex, "CWeaponCSBase", "m_flRecoilIndex");
	NETVAR(int, m_iViewModelIndex, "CBaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int, m_iWorldModelIndex, "CBaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int, m_iWorldDroppedModelIndex, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "CBaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "CBaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flThrowStrength, "CBaseCSGrenade", "m_flThrowStrength");
	NETVAR(int, m_nExplodeEffectTickBegin, "CBaseCSGrenadeProjectile", "m_nExplodeEffectTickBegin");
	NETVAR(float_t, m_flPostponeFireReadyTime, "CBaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<c_base_weapon_world_model>, m_hWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(int, m_zoomLevel, "CWeaponCSBaseGun", "m_zoomLevel");

	CCSWeaponInfo* GetWeaponData();
	bool HasBullets();
	char* GetGunIcon();
	float GetGunStringSize();
	bool CanFire();
	bool IsGrenade();
	bool IsZeus();
	bool IsKnife();
	bool IsReloading();
	bool IsPistol();
	bool IsSniper();
	bool IsShotgun();
	bool IsSmoke();
	bool IsFlash();
	bool IsKnifeOrGrenade();
	bool RequiresRecoilControl();

	float GetInaccuracy();
	float GetSpread();
	void  UpdateAccuracyPenalty();
	bool check_detonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval);
};

class c_base_player : public c_base_entity
{
public:

	NETVAR(bool, m_bHasDefuser, "CCSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bGunGameImmunity, "CCSPlayer", "m_bGunGameImmunity");
	NETVAR(int, m_iShotsFired, "CCSPlayer", "m_iShotsFired");
	NETVAR(QAngle, m_angEyeAngles, "CCSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "CCSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "CCSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "CCSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "CCSPlayer", "m_bIsScoped");
	NETVAR(bool, m_bIsDefusing, "CCSPlayer", "m_bIsDefusing");
	NETVAR(float, m_flLowerBodyYawTarget, "CCSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int, m_iHealth, "CBasePlayer", "m_iHealth");
	NETVAR(int, m_lifeState, "CBasePlayer", "m_lifeState");
	NETVAR(int, m_fFlags, "CBasePlayer", "m_fFlags");
	NETVAR(int, m_nTickBase, "CBasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "CBasePlayer", "m_vecViewOffset[0]");
	NETVAR(QAngle, m_viewPunchAngle, "CBasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "CBasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<c_base_view_model>, m_hViewModel, "CBasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "CBasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "CBasePlayer", "m_flMaxspeed");
	NETVAR(int, m_iObserverMode, "CBasePlayer", "m_iObserverMode");
	NETVAR(CHandle<c_base_player>, m_hObserverTarget, "CBasePlayer", "m_hObserverTarget");
	NETVAR(float, m_flFlashMaxAlpha, "CCSPlayer", "m_flFlashMaxAlpha");
	NETVAR_OFFSET(float, m_flFlashMaxAlphaOffset, "CCSPlayer", "m_flFlashMaxAlpha", -0x8);
	NETVAR(int, m_nHitboxSet, "CBaseAnimating", "m_nHitboxSet");
	NETVAR(int, m_nForceBone, "CBaseAnimating", "m_nForceBone");
	NETVAR(bool, m_bClientSideAnimation, "CBaseAnimating", "m_bClientSideAnimation");
	NETVAR(CHandle<c_base_combat_weapon>, m_hActiveWeapon, "CBaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(int, m_iAccount, "CCSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "CCSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "CBaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "CServerAnimationData", "m_flCycle");
	NETVAR(int, m_nSequence, "CBaseViewModel", "m_nSequence");
	PNETVAR(char, m_szLastPlaceName, "CBasePlayer", "m_szLastPlaceName");
	NETVAR(QAngle, m_angAbsAngles, "CBaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "CBaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "CBaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "CBaseEntity", "m_flDuckAmount");
	NETVAR(int, m_nSurvivalTeam, "CCSPlayer", "m_nSurvivalTeam");
	NETVAR(int, m_iFOV, "CBasePlayer", "m_iFOV");
	NETVAR(int, m_iDefaultFOV, "CBasePlayer", "m_iDefaultFOV");
	PNETVAR(CHandle<c_base_combat_weapon>, m_hMyWeapons, "CBaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CBaseHandle, m_hMyWearables, "CBaseCombatCharacter", "m_hMyWearables");
	NETVAR_OFFSET(QAngle*, GetVAngles, "CBasePlayer", "deadflag", +0x4);
	NETVAR(CBaseHandle, m_hGroundEntity, "CBasePlayer", "m_hGroundEntity");
	NETVAR(int[MAX_PATCHES], m_vecPlayerPatchEconIndices, "CCSPlayer", "m_vecPlayerPatchEconIndices");
	NETVAR(float, m_flNextAttack, "CBaseCombatCharacter", "m_flNextAttack");

	std::vector<int> hitboxes();

	Vector        GetEyePos();
	Vector		  get_hitbox_position(int hitbox_id);
	Vector        get_bone_position(int bone);
	player_info_t GetPlayerInfo();
	bool          IsAlive();
	bool		  IsDead();
	bool		  IsDying();
	bool		  IsFlashed();
	bool          IsEnemy();
	bool          HasC4();
	bool		  InAir();
	int& m_nMoveType();
	QAngle& GetAbsAngles();
	void SetAbsAngles(const QAngle& wantedang);
	void UpdateClientSideAnimation();
	void InvalidateBoneCache();
	
	CAnimationLayer* GetAnimOverlay(int i);
	CAnimationLayer* GetAnimOverlays();

	CAnimState* GetPlayerAnimState();

	void ResetAnimationState(CAnimState* state);

	static __forceinline c_base_player* GetPlayerByUserId(int id)
	{
		return static_cast<c_base_player*>(GetEntityByIndex(g::engine_client->GetPlayerForUserID(id)));
	}

	static __forceinline c_base_player* GetPlayerByIndex(int i)
	{
		return static_cast<c_base_player*>(GetEntityByIndex(i));
	}
};

class c_base_view_model : public c_base_entity
{
public:
	NETVAR(int, m_nModelIndex, "CBaseViewModel", "m_nModelIndex");
	NETVAR(int, m_nViewModelIndex, "CBaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<c_base_combat_weapon>, m_hWeapon, "CBaseViewModel", "m_hWeapon");
	NETVAR(CHandle<c_base_player>, m_hOwner, "CBaseViewModel", "m_hOwner");
	NETPROP(m_nSequence, "CBaseViewModel", "m_nSequence");

	void SetModelIndex(int sequence);
	void SendViewModelMatchingSequence(int sequence);
};

class c_player_resource
{
public:
	NETVAR(int[MAX_PLAYERS], GetPing, "CPlayerResource", "m_iPing");
};

class CSPlayerResource : public c_player_resource
{
public:
	NETVAR(int[MAX_PLAYERS], GetRank, "CCSPlayerResource", "m_iCompetitiveRanking");
	NETVAR(int[MAX_PLAYERS], GetWins, "CCSPlayerResource", "m_iCompetitiveWins");
	NETVAR(int[MAX_PLAYERS], GetLevel, "CCSPlayerResource", "m_nPersonaDataPublicLevel");
	NETVAR(int[MAX_PLAYERS], GetTeamColor, "CCSPlayerResource", "m_iCompTeammateColor");
	NETVAR(char[MAX_PLAYERS][16], GetClanTag, "CCSPlayerResource", "m_szClan");
	NETVAR(unsigned[MAX_PLAYERS], GetCoin, "CCSPlayerResource", "m_nActiveCoinRank");
	NETVAR(unsigned[MAX_PLAYERS], GetMusicKit, "CCSPlayerResource", "m_nMusicID");
	NETVAR(Vector, m_bombsiteCenterA, "CCSPlayerResource", "m_bombsiteCenterA");
	NETVAR(Vector, m_bombsiteCenterB, "CCSPlayerResource", "m_bombsiteCenterB");
};

class CSGameRulesProxy
{
public:
	NETVAR(bool, m_bBombPlanted, "CCSGameRulesProxy", "m_bBombPlanted");
	NETVAR(uint32_t, m_iRoundTime, "CCSGameRulesProxy", "m_iRoundTime");
	PNETVAR(bool, m_bIsValveDS, "CCSGameRulesProxy", "m_bIsValveDS");
};