#pragma once
#define NOMINMAX
#include <Windows.h>
#include "../../sdk/math/Viewmatrix.h"
#include "../../sdk/math/Vector.h"

namespace globals
{
	extern ImDrawList* draw_list;

	extern HWND hwnd;

	extern VMatrix* viewmatrix;

	extern int width;
	extern int height;

	extern bool can_unhook;

	bool world2screen(const Vector& in, Vector& out);
}

