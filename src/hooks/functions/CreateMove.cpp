#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        original_fn(rcx, slot, active);

    features::aimbot::handle(nullptr); //TODO: get CUserCmd

    original_fn(rcx, slot, active);
}
