#include "../hooks.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../sdk/helpers/utils.h"

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
	//void* rcx, CViewSetup* rdx, VMatrix* world_to_view, VMatrix* view_to_projection, VMatrix* world_to_projection, VMatrix* world_to_pixels
	void __fastcall get_matrices_for_view::hooked(SafetyHookContext& ctx)
	{
		VMatrix* world_to_projection = utils::get_context_argument<VMatrix*, 5>(&ctx);
		
		get_viewmatrix(world_to_projection);
	}
}