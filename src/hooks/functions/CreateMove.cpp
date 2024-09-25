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

    for (auto& x : m_player_data)
    {
        if (!x.m_PlayerPawn)
            continue;

        auto eye_pos = local_pawn->GetEyePos();

        Vector entity_eye_pos = x.m_vecEyePos;

        TraceFilter_t filter(0x1C3003, local_pawn, nullptr, 4); //4
        Ray_t ray;
        Trace_t trace;

        Vector start = eye_pos;
        Vector end = entity_eye_pos;

        ray.start = start;
        ray.end = end;

        bool result = g::game_trace->TraceShape(&ray, start, end, &filter, &trace); //not working quite right

        if (result)
        {
            bool did_hit = trace.m_flFraction > 0.97f;

            printf("did hit? : %d\n", did_hit);
        }
    }
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
