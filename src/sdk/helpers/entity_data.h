#pragma once
#include <list>
#include <mutex>
#include <vector>
#include <map>
#include <array>
#include <bitset>
#include "../classes/CCSPlayerController.h"
#include "../classes/CCSPlayerPawn.h"
#include "../classes/CBasePlayerWeapon.h"
#include "../helpers/BBox_t.h"
#include "../math/QAngle.h"
#include "../sdk.h"
#include "../classes/CHandle.h"
#include "../../sdk/interfaces/CSGOInput.h"

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

		std::array<hitbox_info_t, HITBOX_MAX> hitboxes;
		std::list<bone_info_t> bones_w2s;
	};

	struct entry_data_t
	{
		std::list<player_data_t> player_data;
	};

	struct EntityInstance_t
	{
		CEntityInstance* entity;
		CHandle handle;
	};

	extern std::list<EntityInstance_t> player_instances;
	extern std::list<EntityInstance_t> entity_instances;
	extern std::list<entry_data_t> player_entry_data;

	extern std::mutex locker;
	
	namespace view_matrix
	{
		extern VMatrix* matrix;
	};

	void fetch_player_data(CUserCmd* cmd);
	void destroy();
}

