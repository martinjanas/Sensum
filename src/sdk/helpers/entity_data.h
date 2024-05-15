#pragma once
#include <list>
#include <mutex>
#include <vector>
#include <map>
#include <array>
#include "../classes/CCSPlayerController.h"
#include "../classes/CCSPlayerPawn.h"
#include "../helpers/BBox_t.h"
#include "../math/QAngle.h"

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
		const char* player_name;
		uint32_t index;
		Vector m_vecOrigin;
		int m_iHealth;
		int m_iShotsFired;
		int clip;
		CModelState* model_state;
		CStrongHandle<CModel> model;
		BBox_t bbox;
		BBox_t head_bbox;
		CCSPlayerPawn* pawn;
		CCSPlayerPawn* localplayer_pawn;
		ABBox_t abbox;
		Vector local_eyepos;

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

	void fetch_player_data();
	void destroy();
}

