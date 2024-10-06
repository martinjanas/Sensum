#include "globals.h"
#include "../../hooks/hooks.h"
#include "../helpers/entity_data.h"
#include <d3d9types.h>

namespace globals
{
	ImDrawList* draw_list = nullptr;

	HWND hwnd = NULL;

	int width = -1;
	int height = -1;

	bool can_unhook = false;

	bool world2screen(const Vector& in, Vector& out)
	{
		if (!entity_data::view_matrix::matrix)
			return false;

		const auto& world_to_pixels = entity_data::view_matrix::matrix;

		out.x = world_to_pixels->m[0][0] * in.x + world_to_pixels->m[0][1] * in.y + world_to_pixels->m[0][2] * in.z + world_to_pixels->m[0][3];
		out.y = world_to_pixels->m[1][0] * in.x + world_to_pixels->m[1][1] * in.y + world_to_pixels->m[1][2] * in.z + world_to_pixels->m[1][3];
		out.z = world_to_pixels->m[2][0] * in.x + world_to_pixels->m[2][1] * in.y + world_to_pixels->m[2][2] * in.z + world_to_pixels->m[2][3];

		float w = world_to_pixels->m[3][0] * in.x + world_to_pixels->m[3][1] * in.y + world_to_pixels->m[3][2] * in.z + world_to_pixels->m[3][3];

		if (w < 0.01f)
			return false;

		out.x /= w;
		out.y /= w;
		out.z /= w; 

		const int width = globals::width;
		const int height = globals::height;

		out.x = (width * 0.5f) + (out.x * width) * 0.5f;
		out.y = (height * 0.5f) - (out.y * height) * 0.5f;

		return true;
	}
}
