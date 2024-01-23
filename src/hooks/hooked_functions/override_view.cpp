#include "../hooks.h"
#include "../../features/features.h"
#include "../../settings/globals.h"

namespace hooks
{
	void __stdcall override_view::hooked(CViewSetup* view)
	{
		original(g::client_mode, view);

		color_modulation::handle();

		features::thirdperson();

		if (!globals::view_matrix::has_offset)
		{
			globals::view_matrix::has_offset = true;
			globals::view_matrix::offset = (reinterpret_cast<DWORD>(&g::engine_client->WorldToScreenMatrix()) + 0x40);
		}

		if (g::local_player && !g::local_player->m_bIsScoped() && g::local_player->IsAlive())
			view->fov = settings::misc::debug_fov;
	}
}