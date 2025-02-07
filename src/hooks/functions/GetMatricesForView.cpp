#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"

static void get_viewmatrix(VMatrix* viewmatrix)
{
	if (!g::engine_client->IsInGame())
		return;
	
	CCSPlayerController* controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
	if (!controller)
		return;

	auto pawn = reinterpret_cast<CCSPlayerPawn*>(g::entity_system->GetEntityFromHandle(controller->m_hPawn()));
	if (!pawn)
		return;

	if (!entity_data::view_matrix::matrix)
		entity_data::view_matrix::matrix = viewmatrix;
}

namespace hooks
{
	void __fastcall get_matrices_for_view::hooked(void* rcx, CViewSetup* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels)
	{
		safetyhook.fastcall<void>(rcx, rdx, world_to_view, view_to_projection, world_to_projection, world_to_pixels);
		
		get_viewmatrix(world_to_projection);
	}
}