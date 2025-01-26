#pragma once
#include "../thirdparty/ImGui/imgui.h"
#include <map>

enum ETargetSelection
{
	TARGET_HEAD = (1 << 0),
	TARGET_CHEST = (1 << 1),
	TARGET_ARMS = (1 << 2),
	TARGET_LEGS = (1 << 3),
	TARGET_NEAREST_IN_AIR = (1 << 4)
};

namespace settings
{
	namespace aimbot
	{
		struct weapon_config_t
		{
			bool enabled = true;
			bool nearest_hitbox_in_air_override = true;
			float fov = 20.f;
			float smooth = 3.5f;
			float smooth_time = 1.f;
			int smooth_mode = 1;
			int hitboxes = TARGET_HEAD | TARGET_CHEST;

			struct
			{
				bool enabled = true;
				float pitch = 2.f;
				float yaw = 2.f;
			} recoil;
		};

		extern std::map<int, weapon_config_t> weapon_configs;
	}

	namespace misc
	{
		extern bool fov_changer;
		extern int fov;
		extern bool bhop;
		extern float rotation_x;
		extern float rotation_y;
		extern float rotation_z;
	}

	namespace esp
	{
		extern bool bone_esp;
		extern bool box_esp;
		extern bool name_esp;
		extern bool health_esp;
		extern bool visible_only;

		extern ImVec4 bone_clr;
		extern ImVec4 box_clr;
		extern ImVec4 health_clr;
		extern ImVec4 name_clr;
	}
}





