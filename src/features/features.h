// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

namespace features
{
	namespace esp
	{
		void render();
		void render_entities();

		void bone_esp(entity_data::player_data_t& data);
		void name_esp(entity_data::player_data_t& data, const BBox_t& bbox);
		void weapon_esp(entity_data::player_data_t& data);
		void draw_fov(entity_data::player_data_t& data);
	}

	namespace aimbot
	{
		struct target_info_t
		{
			bool operator<(const target_info_t& other) const
			{
				return fov < other.fov;
			}

			CCSPlayerPawn* pawn;
			entity_data::player_data_t* player_data;

			float fov;
			float distance;
			bool in_air;

			QAngle target_angle;
			const char* name;
		};
		
		void handle(CUserCmd* cmd);
	}

	namespace bhop
	{
		void handle(CUserCmd* cmd);
	}
}
