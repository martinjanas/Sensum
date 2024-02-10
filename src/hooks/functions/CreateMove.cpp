#include "../hooks.h"
#include "../../sdk/sdk.h"

#include "../../sdk/classes/CCSPlayerPawn.h"
#include "../../sdk/classes/CCSPlayerController.h"
#include "../../sdk/helpers/entity_data.h"

namespace Aimbot
{
    void Aim()
    {
        if (!g::engine_client->IsInGame())
            return;

        for (auto& data : entity_data::player_entry_data.front().player_data)
        {
            if (data.index == 0)
                continue;

            if (data.m_iHealth <= 0)
                continue;

            QAngle viewangles;
            g::client->GetViewAngles(0, &viewangles);

            auto delta_x = viewangles.pitch - data.aimpos.pitch;
            auto delta_y = viewangles.yaw - data.aimpos.yaw;

            float fov = std::hypotf(delta_x, delta_y);

            if (GetAsyncKeyState(VK_LBUTTON) && (fov < 3.f))
                g::client->SetViewAngles(0, data.aimpos);
        }

    }
}

void __fastcall hooks::create_move::hooked(CSGOInput* input, unsigned int a2, void* a3, unsigned __int8 unk)
{
    static auto gadget_address = modules::client.pattern_scanner.scan("FF 23").as();

    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        original_fn(input, a2, a3, unk);
       
    //Aimbot::Aim();

    entity_data::fetch_player_data();

    original_fn(input, a2, a3, unk);
}