#pragma once
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/helpers/globals.h"

namespace features
{
	namespace esp
	{
		void render();

		void bone_esp(entity_data::player_data_t& data);
		void name_esp(entity_data::player_data_t& data, const Vector& origin_out, const Vector& screen_head_pos);
	}

	namespace aimbot
	{
		void handle(CUserCmd* cmd);
	}
}
