#pragma once
#include "../thirdparty/ImGui/imgui.h"

namespace settings
{
	namespace aimbot //just for now, gonna move to weapon based settings later
	{
		extern int fov;
		extern int hitboxes;
		extern int smooth_mode;
		extern float smooth;
		extern float pitch;
		extern float yaw;
	}

	namespace misc
	{
		extern bool fov_changer;
		extern int fov;
		extern bool bhop;
	}

	namespace esp
	{
		extern bool bone_esp;
		extern bool box_esp;
		extern bool name_esp;
		extern bool health_esp;

		extern ImVec4 bone_clr;
		extern ImVec4 box_clr;
		extern ImVec4 health_clr;
		extern ImVec4 name_clr;
	}
}





