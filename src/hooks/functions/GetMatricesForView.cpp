#include "../hooks.h"
#include "../../../../features/entity_data.h"
namespace hooks
{
	void __fastcall get_matrices_for_view::hooked(void* rcx, void* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels)
	{
		if (g::engine_client->IsInGame() && world_to_projection)
		{
			if (!entity_data::view_matrix::matrix)
				entity_data::view_matrix::matrix = world_to_projection;
		}

		safetyhook.fastcall<void>(rcx, rdx, world_to_view, view_to_projection, world_to_projection, world_to_pixels);
	}
}