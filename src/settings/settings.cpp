#include "../settings/settings.h"

namespace settings
{
	namespace aimbot
	{
		std::map<int, weapon_config_t> weapon_configs;
	}

	namespace misc
	{
		bool fov_changer = false;
		int fov = 90;
		bool bhop = false;
		float rotation_x = 0;
		float rotation_y = 0;
		float rotation_z = 0;
	}

	namespace esp
	{
		bool bone_esp = true;
		bool box_esp = true;
		bool name_esp = false;
		bool health_esp = false;
		bool visible_only = true;

		ImVec4 bone_clr = ImVec4(255, 255, 255, 255);
		ImVec4 box_clr = ImVec4(255, 255, 255, 255);
		ImVec4 health_clr = ImVec4(255, 255, 255, 255);
		ImVec4 name_clr = ImVec4(255, 255, 255, 255);
	}
}