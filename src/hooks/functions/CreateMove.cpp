#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../features/features.h"



void trace_shit()
{
    std::list<entity_data::player_data_t> m_player_data;

    if (!g::engine_client->IsInGame())
        return;

    if (entity_data::locker.try_lock())
    {
        std::lock_guard<std::mutex> lock(entity_data::locker, std::adopt_lock);

        m_player_data.clear();

        if (!entity_data::player_entry_data.empty())
            std::ranges::copy(entity_data::player_entry_data.front().player_data, std::back_inserter(m_player_data));
    }

    auto local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
    if (!local_controller)
        return;

    auto local_pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPlayerPawn());
    if (!local_pawn)
        return;

    Vector start = local_pawn->GetEyePos();
    Vector forward = local_pawn->m_angEyeAngles().to_vector();
    forward.normalize();

    Vector end = start + forward * 2048.f; 

    //TraceFilter_t filter(0x1C3003 | CONTENTS_HITBOX | CONTENTS_PLAYERCLIP, local_pawn, nullptr, 4); //4 //0x1C3003
    //Ray_t ray;
    //ray.Init(start, end);

    //Trace_t trace;
    //g::game_trace->TraceShape(&ray, start, end, &filter, &trace);

    //printf("fraction %.1f\n", trace.m_flFraction);

    //if (trace.m_pHitEntity)
    //{
    //    auto info = trace.m_pHitEntity->GetSchemaClassInfo();

    //    if (info)
    //        printf("name %s\n", info->m_pszName);
    //}

    //for (auto& x : m_player_data)
    //{
    //    if (!x.m_PlayerPawn)
    //        continue;

    //    //bool result = g::game_trace->ClipRayToEntity(&ray, start, end, x.m_PlayerPawn, &filter, &trace); //crashing

    //    //if (result)
    //    //{
    //    //    bool did_hit = trace.m_flFraction > 0.97f;

    //    //    printf("start: %.1f, %.1f, %.1f\nend: %.1f, %.1f, %.1f\npos: %.1f, %.1f, %.1f\n", trace.m_vecStartPos.x, trace.m_vecStartPos.y, trace.m_vecStartPos.z, trace.m_vecEndPos.x, trace.m_vecEndPos.y, trace.m_vecEndPos.z, trace.m_vecPosition.x, trace.m_vecPosition.y, trace.m_vecPosition.z);

    //    //    /*if (did_hit && trace.m_pHitEntity)
    //    //    {
    //    //        auto controller = (CCSPlayerController*)trace.m_pHitEntity;

    //    //        const char* name = controller->m_iszPlayerName();

    //    //        printf("hit_entity valid, name: %s\n", name);
    //    //    }*/
    //    //}
    //}
}

bool hooks::clientmode_createmove::hooked(void* rcx)
{
    auto cmd = g::csgo_input->GetUserCmd();

    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return original_fn(rcx);
 
    features::aimbot::handle(cmd);

    trace_shit();

    return original_fn(rcx);
}
