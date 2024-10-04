#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    auto _cmd = g::csgo_input->GetUserCmd();

    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        original_fn(rcx, slot, active);
 
    features::aimbot::handle(_cmd);

    original_fn(rcx, slot, active);
}

