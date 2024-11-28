#include "../settings/settings.h"

namespace settings
{
	namespace aimbot
	{
		int fov = 20;
		int hitboxes = TARGET_HEAD | TARGET_CHEST;
		int smooth_mode = 1;
		float smooth = 3.5f;
		float pitch = 1.5f;
		float yaw = 1.5f;

		std::map<int, weapon_config_t> weapon_configs;
	}

	namespace misc
	{
		bool fov_changer = false;
		int fov = 90;
		bool bhop = false;
	}

	namespace esp
	{
		bool bone_esp = true;
		bool box_esp = false;
		bool name_esp = false;
		bool health_esp = false;
		bool visible_only = true;

		ImVec4 bone_clr = ImVec4(255, 255, 255, 255);
		ImVec4 box_clr = ImVec4(255, 255, 255, 255);
		ImVec4 health_clr = ImVec4(255, 255, 255, 255);
		ImVec4 name_clr = ImVec4(255, 255, 255, 255);
	}
}