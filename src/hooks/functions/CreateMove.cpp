#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    features::aimbot::handle(nullptr); //TODO: get CUserCmd

    original_fn(rcx, slot, active);
}
