#include "../hooks.h"
#include "../../settings/settings.h"
#include "../../helpers/input.h"

namespace hooks
{
	void __stdcall paint_traverse::hooked(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		original(g::vgui_panel, panel, forceRepaint, allowForce);

		auto panel_id = fnv::hash_runtime(g::vgui_panel->GetName(panel));

		if (settings::misc::noscope)
		{
			if (g::engine_client->IsInGame() && g::engine_client->IsConnected() && g::local_player->IsAlive() && panel_id == FNV("HudZoom"))
				return;
		}
	}
}