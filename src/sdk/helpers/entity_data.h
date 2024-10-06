#pragma once
#include <list>
#include <mutex>
#include <vector>
#include <map>
#include <array>
#include "../classes/CCSPlayerController.h"
#include "../classes/CCSPlayerPawn.h"
#include "../classes/CBasePlayerWeapon.h"
#include "../helpers/BBox_t.h"
#include "../math/QAngle.h"
#include "../sdk.h"
#include "../classes/CHandle.h"
#include "../../sdk/interfaces/CSGOInput.h"

namespace entity_data
{
	struct hitbox_info_t
	{
		Vector hitbox_pos;
		uint16_t index;
		const char* entity_name;
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
		bool is_visible;
		Trace_t trace;

		std::array<hitbox_info_t, HITBOX_MAX> hitboxes;
	};

	struct entry_data_t
	{
		std::list<player_data_t> player_data;
	};

	struct instance_t
	{
		CEntityInstance* entity;
		CHandle handle;
	};

	extern std::list<instance_t> player_instances;
	extern std::list<instance_t> entity_instances;
	extern std::list<entry_data_t> player_entry_data;

	extern std::mutex locker;
	
	namespace view_matrix
	{
		extern VMatrix* matrix;
		extern std::mutex mutex;
	};

	void fetch_player_data(CUserCmd* cmd);
	void destroy();
}

