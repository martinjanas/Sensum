#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

bool __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, CUserCmd* cmd)
{
    const bool original = original_fn(rcx, slot, cmd);
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected()) {
        return original;
    }

    features::aimbot::handle(cmd);
    features::movement::handle(cmd);
    
    return original_fn(rcx, slot, cmd);
}
