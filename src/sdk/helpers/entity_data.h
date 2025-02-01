#pragma once
#include <list>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <map>
#include <array>
#include <bitset>
#include <variant>

#include "IconFetcher.h"
#include "../classes/CCSPlayerController.h"
#include "../classes/CCSPlayerPawn.h"
#include "../classes/CBasePlayerWeapon.h"
#include "../helpers/BBox_t.h"
#include "../math/QAngle.h"
#include "../sdk.h"
#include "../classes/CHandle.h"
#include "../../sdk/interfaces/CSGOInput.h"
#include "../../sdk/classes/GameTick_t.h"

enum PLAYER_FLAGS : uint32_t
{
	PLAYER_VISIBLE,
	PLAYER_IN_SMOKE,
	PLAYER_IN_AIR,
	PLAYER_ALIVE
};

namespace entity_data
{
	struct hitbox_info_t
	{
		Vector hitbox_pos;
		uint16_t index;
		bool visible;
	};

	struct bone_info_t
	{
		Vector bone;
		Vector bone_parent;
		bool got_bone;
		bool got_parent;
	};

	struct local_data_t
	{

	};
	
	struct player_data_t
	{
		const char* m_szPlayerName;
		uint32_t m_iPlayerIndex;
		Vector m_vecOrigin;
		Vector m_vecAbsOrigin;
		Vector m_vecEyePos;
		Vector m_vecOldOrigin;
		QAngle m_vecOldEyeAngles;
		QAngle m_vecEyeAngles;
		int m_iHealth;
		int m_iShotsFired;
		int m_iClip1;
		CModelState m_ModelState;
		CStrongHandle<CModel> m_hModel;
		BBox_t bbox;
		CCSPlayerPawn* m_PlayerPawn;
		Transform_t hitbox_transform[HITBOX_MAX];
		std::bitset<32> flags;

		icon_t weapon_icon;
		const char* weapon_text;
		
		std::array<hitbox_info_t, HITBOX_MAX> hitboxes;
		std::list<bone_info_t> bones_w2s;
	};

	struct bomb_info_t
	{
		bool m_bBombTicking;
		int m_nBombSite;
		float m_flC4Blow;
		Vector m_vecAbsOrigin;
		icon_t bomb_icon;
	};

	struct generic_entity_info_t
	{
		Vector m_vecOrigin;
	};

	enum EGrenadeType
	{
		GRENADE_SMOKE,
		GRENADE_FLASH,
		GRENADE_MOLOTOV,
		GRENADE_HE,
		GRENADE_DECOY
	};

	struct smoke_info_t
	{
		int m_nSmokeEffectTickBegin;
		bool m_bDidSmokeEffect;
		float remaining_smoke_time;
	};

	struct molotov_info_t
	{
		float burn_time;
	};

	struct hegrenade_info_t
	{
		int m_nExplodeEffectTickBegin;
	};

	struct flashbang_info_t
	{
		float flash_time;
	};
	
	struct grenade_info_t : generic_entity_info_t
	{
		EGrenadeType type;
		icon_t icon;

		std::variant<std::monostate, hegrenade_info_t, flashbang_info_t, smoke_info_t, molotov_info_t> specific_data;
	};

	struct world_entity_info_t : generic_entity_info_t
	{
		const char* name;
		icon_t icon;
	};

	struct player_entry_data_t
	{
		std::list<player_data_t> player_data;
	};

	struct entity_entry_data_t
	{
		std::list<bomb_info_t> bomb_info;
		std::list<grenade_info_t> grenade_info;
		std::list<world_entity_info_t> dropped_ent_info;
	};

	struct EntityInstance_t
	{
		CEntityInstance* entity;
		CHandle handle;
	};


	extern std::list<EntityInstance_t> player_instances;
	extern std::list<EntityInstance_t> entity_instances;
	extern std::list<player_entry_data_t> player_entry_data;
	extern std::list<entity_entry_data_t> entity_entry_data;

	extern std::shared_mutex player_locker;
	extern std::shared_mutex entity_locker;
	
	namespace view_matrix
	{
		extern VMatrix* matrix;
	};

	void fetch_player_data();
	void fetch_entity_info();
	void destroy();
}

