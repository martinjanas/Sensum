#include "../features/features.h"

void features::bhop::handle(CUserCmd* cmd)
{
    if (!g::engine_client->IsInGame() || !settings::misc::bhop)
        return;

    CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
    if (!localplayer)
        return;

    CCSPlayerPawn* localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPawn());
    if (!localpawn)
        return;

    if (!localpawn->IsAlive())
        return;

    if (cmd->nButtons.nValue & IN_JUMP && !localpawn->InAir())
        cmd->nButtons.nValue &= ~IN_JUMP;
}