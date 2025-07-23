#include "../hooks.h"
#include "../../sdk/cheat.h"

int64_t* __fastcall hooks::level_shutdown::hooked(void* rcx)
{
	cheat::interfaces().invalidate_interfaces();

	return original_fn(rcx);
}