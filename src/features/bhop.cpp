#include "../features/features.h"
#include "../sdk/cheat.h"

void features::bhop::handle(CUserCmd* cmd)
{
    if (!cheat::interfaces().engine->IsInGame() || !settings::misc::bhop)
        return;

    CCSPlayerController* localplayer = cheat::interfaces().entity_system->GetLocalPlayerController<CCSPlayerController*>();
    if (!localplayer)
        return;

    CCSPlayerPawn* localpawn = cheat::interfaces().entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPawn());
    if (!localpawn)
        return;

    if (!localpawn->IsAlive())
        return;

    if (cmd->nButtons.nValue & IN_JUMP && !localpawn->InAir())
        cmd->nButtons.nValue &= ~IN_JUMP;
}