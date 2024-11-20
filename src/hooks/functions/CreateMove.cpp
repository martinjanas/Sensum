#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

bool __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, CUserCmd* cmd) //CUserCmd is null, todo: take a look in ida
{
    const auto result = original_fn(rcx, slot, cmd);

    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return result;
 
    features::aimbot::handle(cmd);

    return result;
}

