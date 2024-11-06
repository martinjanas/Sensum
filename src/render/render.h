#pragma once
#include "../thirdparty/ImGui/imgui.h"
#include "../thirdparty/ImGui/imgui_internal.h"
#include "../thirdparty/ImGui/imgui_freetype.h"
#include "../render/fonts/droidsans.hpp"

namespace render
{
	namespace fonts
	{
		extern ImFont* header_title;
		extern ImFont* header_buttons;
	}

	void init_fonts();
}