#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"
#include "../../sdk/helpers/CUtlBuffer.h"
void __fastcall hooks::createmove_csgoinput::hooked(void* rcx, int slot, bool active)
{
    original_fn(rcx, slot, active);

    entity_data::fetch_player_data();
    //entity_data::fetch_entity_info();

    //TODO: GlobalVars struct is outdated
    /*if (g::global_vars)
    {
        g_Console->println("curtime: %.4f", g::global_vars->GetCurtime());
    }*/
}

bool __fastcall hooks::createmove_csgoinput21::hooked(void* rcx, int slot, CUserCmd* cmd, float curtime)
{
    bool status = original_fn(rcx, slot, cmd, curtime);

    return status;
}
