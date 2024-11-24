#include "../hooks.h"
#include "../../settings/settings.h"

namespace hooks
{
	float __fastcall hooks::get_fov::hooked(void* camera)
	{
		if (g::engine_client->IsInGame() && settings::misc::fov_changer)
			return static_cast<float>(settings::misc::fov);

		return safetyhook.fastcall<float>(camera);
	}
}