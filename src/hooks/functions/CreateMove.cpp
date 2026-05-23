#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"
#include "../../sdk/helpers/CUtlBuffer.h"
/*
    MJ: Hooking two createmoves now, because aimbot & rcs is behaving weirdly in createmove_csgoinput21

    TODO: Fix crc?
*/
static inline CUserCmd* g_cmd = nullptr;

void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        original_fn(rcx, slot, active);
    
    entity_data::fetch_player_data();
    //entity_data::fetch_entity_info();

    /*if (g::global_vars)
    {
        g_Console->println("frametime: %.4f, frametime2: %.4f", g::global_vars->m_frametime, g::global_vars->m_frametime2);
    }*/

    original_fn(rcx, slot, active);
}

bool __fastcall hooks::createmove_csgoinput21::hooked(void* rcx, int slot, CUserCmd* cmd, float curtime)
{
    g_cmd = cmd;

    return original_fn(rcx, slot, cmd, curtime);
}
