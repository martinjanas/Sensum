#include "../hooks.h"
#include "../../settings/settings.h"

namespace hooks
{
	float __fastcall hooks::get_fov::hooked(void* camera)
	{
		if (g::engine_client->IsInGame() && settings::visuals::m_bFovChanger)
			return static_cast<float>(settings::visuals::m_iFov);

		return safetyhook.fastcall<float>(camera);
	}
}