#include "render.h"
#include <Windows.h>
#include <iostream>

namespace render
{
	namespace fonts
	{
		ImFont* header_title;
		ImFont* header_buttons;
		ImFont* selector;
	}

	void init_fonts()
	{
		ImFontConfig font_config;
		font_config.OversampleH = 1;
		font_config.OversampleV = 1;
		font_config.PixelSnapH = true;
		font_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_ForceAutoHint;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF,
			0x0400, 0x044F,
			0
		};

		auto io = &ImGui::GetIO();

		fonts::header_title = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 34.f, &font_config);
		fonts::header_buttons = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 18.5f, &font_config, ranges); //17.f
		fonts::selector = io->Fonts->AddFontFromMemoryCompressedTTF(droidsans_compressed_data, droidsans_compressed_size, 17.f, &font_config, ranges); //17.f
	}
}