#pragma once
#include "render.h"

namespace menu
{
	extern bool is_open;

	void draw();
	void draw_background();
	void render_header();
	void render_bottom_header();

	void modulate_window_alpha();
}
