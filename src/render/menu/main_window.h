#pragma once
#include "../../thirdparty/ImGui/imgui.h"
#include "../../settings/settings.h"

namespace main_window
{
	extern bool is_open;

	extern ImFont* esp;

	void Draw();
	void InitStyle();
}

