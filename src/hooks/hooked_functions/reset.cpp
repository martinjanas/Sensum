#include "../hooks.h"
#include "../../render/render.h"
#include "../../settings/settings.h"

namespace hooks
{
	long __stdcall reset::hooked(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
	{
		if (render::is_ready())
			render::device_lost();

		const auto hr = original(device, params);
		if (hr >= 0 && render::is_ready())
			render::device_reset();

		return hr;
	}
}