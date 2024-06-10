#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../../../features/entity_data.h"
#include "../../sdk/math/math.h"
#include "../../settings/settings.h"

float distance_based_fov(const QAngle& delta, const float& distance)
{
    float pitch_diff = std::sinf(fabsf(delta.pitch) * math::deg2rad) * distance;
    float yaw_diff = std::sinf(fabsf(delta.yaw) * math::deg2rad) * distance;

    float fov = sqrtf(pitch_diff * pitch_diff + yaw_diff * yaw_diff);

    fov = std::clamp<float>(fov, -1, 360.f);

    return fov;
}

namespace Aimbot
{
    static std::list<entity_data::player_data_t> m_player_data;

    static std::vector<int>& get_hitboxes()
    {
        static std::vector<int> list{ HITBOX_HEAD, HITBOX_UPPER_CHEST, HITBOX_PELVIS };
    
        return list;
    }

    void smooth(const float& smooth, QAngle& viewangles, const QAngle& temp, QAngle& out)
    {
        out = temp;
        out.clamp_normalize();

        auto corrected_amount = smooth;
        auto tickrate = (1.0f / 0.015625);

        corrected_amount = tickrate * smooth / 64.f;

        if (corrected_amount < 1.1f)
            return;

        auto temp_v = temp.to_vector();
        auto viewangles_v = viewangles.to_vector();

        auto delta = temp_v - viewangles_v;
        
        auto smoothed = viewangles_v + delta / corrected_amount;
        auto smoothed_v = smoothed.to_qangle();

        out = smoothed_v;
        out.clamp_normalize();
    }

    void Aim(CUserCmd* cmd)
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
        
        float best_fov = 9999.f;
        QAngle best_angle;

        for (const auto& data : m_player_data)
        {
            static int i = 0;

            QAngle viewangles;
            g::client->GetViewAngles(0, &viewangles);

            const auto& hitbox_ids = get_hitboxes();
            if (hitbox_ids.empty())
                continue;

            if (!data.pawn || data.m_iHealth <= 0 || data.hitboxes.empty())
                continue;

            const auto& eye_pos = data.localplayer_pawn->GetEyePos();

            for (const auto& hitbox_id : hitbox_ids)
            {
                for (auto& hitbox_data : data.hitboxes)
                {
                    if (hitbox_data.index != hitbox_id)
                        continue;

                    QAngle target_angle = (hitbox_data.hitbox_pos - eye_pos).to_qangle();
                    target_angle.clamp_normalize();

                    auto delta = target_angle - viewangles;
                    delta.clamp_normalize();

                    float dist = data.m_vecOrigin.dist_to(eye_pos);

                    float fov = distance_based_fov(delta, dist);

                    if (fov < best_fov) 
                    {
                        best_fov = fov; //MJ: Is this really correct?
                        best_angle = target_angle;
                    }

                    //if (i % 25 == 0)
                      //  printf("[%d]: fov: %.1f, best_fov: %.1f\n", hitbox_data.index, fov, best_fov);

                    //smooth(settings::visuals::smooth, viewangles, best_angle, best_angle); //behaves weirdly @ >2 smooth 

                    bool on_button_down = cmd->buttonStates.m_nValueChanged & IN_ATTACK;

                    if (!on_button_down) //This doesnt work properly
                        continue;

                   /* if (!GetAsyncKeyState(VK_LBUTTON))
                        continue;*/

                    const auto& best_angle_vec = best_angle.to_vector();
                    if (!best_angle_vec.is_valid())
                        continue;

                    if (best_fov > settings::visuals::aimbot_fov)
                        continue;

                    g::client->SetViewAngles(0, best_angle);
                }
            }

            i++;
        }
    }
}


void print_values(CUserCmd* cmd)
{
    //const auto& cmd = g::csgo_input->GetUserCmd();
 
    printf("%llu, %llu, %llu\n", cmd->buttonStates.m_nValue, cmd->buttonStates.m_nValueChanged, cmd->buttonStates.m_nValueScroll);
}

CCSPlayerController* controller = nullptr;

bool __fastcall hooks::clientmode_createmove::hooked(void* rcx, CUserCmd* cmd) //CUserCmd* cmd - second argument isnt probably the cmd?
{
    cmd = g::csgo_input->GetUserCmd();

    const auto& result = original_fn(rcx, cmd);
    if (!cmd || (!g::engine_client->IsInGame() || !g::engine_client->IsConnected()))
        return result;

    //print_values(cmd);
  
    Aimbot::Aim(cmd);

    return result;
}
