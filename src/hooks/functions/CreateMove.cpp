#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

bool hooks::clientmode_createmove::hooked(void* rcx)
{
    auto cmd = g::csgo_input->GetUserCmd();

    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return original_fn(rcx);

    features::aimbot::handle(cmd);

    return original_fn(rcx);
}
