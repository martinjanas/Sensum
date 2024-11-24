#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"

/*
    MJ: Hooking two createmoves now, because aimbot & rcs is behaving weirdly in createmove_csgoinput21

    TODO: Fix crc?
*/

static inline CUserCmd* g_cmd = nullptr;

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        original_fn(rcx, slot, active);

    if (g_cmd && g_cmd->csgoUserCmd.pBaseCmd && g_cmd->csgoUserCmd.pBaseCmd->nWeaponSelect == 0)
    {
        features::aimbot::handle(g_cmd);
    }

    original_fn(rcx, slot, active);
}

bool __fastcall hooks::createmove_csgoinput21::hooked(void* rcx, int slot, CUserCmd* cmd, float curtime)
{
    g_cmd = cmd;

    features::bhop::handle(cmd);

    return original_fn(rcx, slot, cmd, curtime);
}
