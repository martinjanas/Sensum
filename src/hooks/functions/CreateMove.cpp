#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

bool __fastcall hooks::clientmodeshared_createmove::hooked(int64_t a1, int64_t a2)
{
    /*auto _cmd = g::csgo_input->GetUserCmd();

    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return original_fn(a1, a2, a3, a4);
 
    features::aimbot::handle(_cmd);*/
       
    printf("hello cmove\n");
    return safetyhook.fastcall<bool>(a1, a2);
}

