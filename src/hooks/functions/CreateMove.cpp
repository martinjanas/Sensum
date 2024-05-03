#include "../hooks.h"
#include "../../sdk/sdk.h"

#include "../../sdk/classes/CCSPlayerPawn.h"
#include "../../sdk/classes/CCSPlayerController.h"
#include "../../sdk/helpers/entity_data.h"

#include "../../sdk/math/math.h"

namespace Aimbot
{
    static std::list<entity_data::player_data_t> m_player_data;

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
        
        QAngle viewangles;
        g::client->GetViewAngles(0, &viewangles);

        for (auto& data : m_player_data)
        {
            if (data.index == 0)
                continue;

            if (data.m_iHealth <= 0)
                continue;

            if (entity_data::hitbox_info.empty())
                continue;

            const auto& eye_pos = data.localplayer_pawn->GetEyePos();

            for (auto& hitboxes : entity_data::hitbox_info) //Redo the hitbox_info_t struct - replace hitbox_pos with Hitbox_t* ?
            {
                auto& hitbox_pos = hitboxes.hitbox_pos;

                QAngle target_angle;
                math::vector2angles(hitbox_pos - eye_pos, target_angle);
                target_angle.ClampNormalize();

                auto delta_x = viewangles.pitch - target_angle.pitch;
                auto delta_y = viewangles.yaw - target_angle.yaw;

                float fov = std::hypotf(delta_x, delta_y);

                if (GetAsyncKeyState(VK_LBUTTON) && (fov < 5.f))
                    g::client->SetViewAngles(0, target_angle);

                entity_data::hitbox_info.clear();
            }
        }

    }
}

bool __fastcall hooks::create_move::hooked(CSGOInput* input, int slot, bool active, std::byte unk) //unk maybe removed?
{
    auto result = original_fn(input, slot, active, unk);

    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        return result;
       
    Aimbot::Aim();

    return result;
}