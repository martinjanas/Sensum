// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include "../settings/settings.h"
#include "../sdk/helpers/entity_data.h"

namespace features
{
	namespace esp
	{
		void render();
		void render_entities(ID3D11Device* device, ID3D11DeviceContext* context);

		void bone_esp(entity_data::player_data_t& data);
		void name_esp(entity_data::player_data_t& data, const BBox_t& bbox);
		void weapon_esp(entity_data::player_data_t& data);
		void draw_fov(entity_data::player_data_t& data);
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
