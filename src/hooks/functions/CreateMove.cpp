#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../sdk/math/math.h"
#include "../../sdk/localplayer.h"
#include "../../settings/settings.h"

float distance_based_fov(const QAngle& delta, const float& distance)
{
    float pitch_diff = std::sinf(fabsf(delta.pitch) * math::deg2rad) * distance;
    float yaw_diff = std::sinf(fabsf(delta.yaw) * math::deg2rad) * distance;

    float fov = sqrtf(pitch_diff * pitch_diff + yaw_diff * yaw_diff);

    fov = std::clamp<float>(fov, -360, 360.f);

    return fov;
}

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
            static int i = 0;

            if (data.index == 0)
                continue;

            if (data.m_iHealth <= 0)
                continue;

            if (entity_data::hitbox_info.empty())
                continue;

            const auto& eye_pos = data.localplayer_pawn->GetEyePos();

            float dist = data.localplayer_pawn->m_pGameSceneNode()->m_vecOrigin().dist_to(data.pawn->m_pGameSceneNode()->m_vecOrigin());

            for (auto& hitboxes : entity_data::hitbox_info) //Redo the hitbox_info_t struct - replace hitbox_pos with Hitbox_t* ?
            {
                auto& hitbox_pos = hitboxes.hitbox_pos;

                QAngle target_angle = (hitbox_pos - eye_pos).to_qangle();
                target_angle.clamp_normalize();

                auto delta = target_angle - viewangles;
                delta.clamp_normalize();

                float fov = distance_based_fov(delta, dist);

                float settings_fov = (settings::visuals::aimbot_fov * 7.5f); //15 distance = ~2 degrees fov

                if (GetAsyncKeyState(VK_LBUTTON) && fabsf(fov) < settings_fov)
                    g::client->SetViewAngles(0, target_angle);
               
                if (i % 25 == 0)
                    printf("fov: %1.f\n", fov);

                entity_data::hitbox_info.clear();

                i++;
            }
        }

    }
}

bool __fastcall hooks::create_move::hooked(CSGOInput* input, int slot, bool active, std::byte unk) //unk maybe removed?
{
    auto result = original_fn(input, slot, active, unk);

    if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
        return result;
       
    //CUserCmd* cmd = input->GetUserCmd(); //CUserCmd and other classes outdated
 
    Aimbot::Aim();

    return result;
}