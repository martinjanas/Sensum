#pragma once
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/helpers/globals.h"
#include <unordered_set>
#include <list>

enum ETargetSelection
{
	TARGET_HEAD = (1 << 0),
	TARGET_CHEST = (1 << 1),
	TARGET_ARMS = (1 << 2),
	TARGET_LEGS = (1 << 3),
	TARGET_NEAREST_IN_AIR = (1 << 4)
};

namespace features
{
	namespace esp
	{
		void render();

		void bone_esp(entity_data::player_data_t& data);
		void name_esp(entity_data::player_data_t& data, const BBox_t& bbox);
	}

	namespace aimbot
	{
		void handle(CUserCmd* cmd);
	}

	namespace bhop
	{
		void handle(CUserCmd* cmd);
	}
}
