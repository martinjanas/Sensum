#include "../hooks.h"

int64_t* __fastcall hooks::level_shutdown::hooked(void* rcx)
{
	sdk::invalidate_interfaces_on_map_unload();

	return original_fn(rcx);
}