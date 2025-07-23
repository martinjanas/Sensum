#include "../hooks.h"
#include "../../sdk/cheat.h"

int64_t* __fastcall hooks::level_init::hooked(void* rcx, const char* map)
{
	cheat::interfaces().validate_interfaces_on_map_load();
	
	return original_fn(rcx, map);
}