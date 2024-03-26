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

        if (entity_data::player_entry_data.empty())
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

            QAngle viewangles;
            g::client->GetViewAngles(0, &viewangles);

            /*auto delta_x = viewangles.pitch - data.aimpos.pitch;
            auto delta_y = viewangles.yaw - data.aimpos.yaw;

            float fov = std::hypotf(delta_x, delta_y);*/

            if (GetAsyncKeyState(VK_LBUTTON)) //&& (fov < 3.f)
                g::client->SetViewAngles(0, data.aimpos);
        }

    }
}

bool __fastcall hooks::create_move::hooked(CSGOInput* input, int slot, bool active, std::byte unk)
{
    const auto result = original_fn(input, slot, active, unk);

    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        return result;
       
    Aimbot::Aim();

    return result;
}