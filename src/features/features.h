#pragma once
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/helpers/globals.h"
#include <unordered_set>

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
}
