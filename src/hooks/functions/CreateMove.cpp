#include "../hooks.h"
#include "../../sdk/sdk.h"

#include "../../sdk/classes/CCSPlayerPawn.h"
#include "../../sdk/classes/CCSPlayerController.h"
#include "../../sdk/helpers/entity_data.h"

namespace Aimbot
{
    std::list<entity_data::player_data_t> m_player_data;

    void Aim()
    {
        if (!g::engine_client->IsInGame())
            return;

        if (entity_data::player_instances.empty())
            return;

        if (entity_data::locker.try_lock())
        {
            m_player_data.clear();
            std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_player_data));
            entity_data::locker.unlock();
        }

        for (auto& data : m_player_data)
        {
            if (data.index == 0)
                continue;

            if (data.m_iHealth <= 0)
                continue;

            if (!GetAsyncKeyState(VK_LBUTTON) & 1)
                continue;

            g::client->SetViewAngles(0, data.aimpos);
        }

    }
}

void __fastcall hooks::create_move::hooked(CSGOInput* input, unsigned int a2, void* a3, unsigned __int8 unk)
{
    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        original_fn(input, a2, a3, unk);
       
    Aimbot::Aim();

    //printf("Hello from create_move\n");

    original_fn(input, a2, a3, unk);
}