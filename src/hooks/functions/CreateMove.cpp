#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

bool __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, CUserCmd* cmd, float curtime) //CUserCmd is null, todo: take a look in ida
{
    features::aimbot::handle(cmd);

    return original_fn(rcx, slot, cmd, curtime);
}

