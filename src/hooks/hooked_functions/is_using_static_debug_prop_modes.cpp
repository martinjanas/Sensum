#include "../hooks.h"
#include "../../settings/settings.h"

namespace hooks
{
	bool _stdcall is_using_static_prop_debug_modes::hooked()
	{
		return settings::visuals::night_mode;
	}
}