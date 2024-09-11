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

    fov = std::clamp<float>(fov, -1, 360.f);

    return fov;
}

namespace Aimbot
{
    static std::list<entity_data::player_data_t> m_player_data;

	std::vector<int>& GetTargetHitboxes()
    {
        static std::vector<int> list{ HITBOX_HEAD, HITBOX_UPPER_CHEST, HITBOX_LOWER_CHEST };
    
        return list;
    }

    void Aim(CUserCmd* cmd)
    {
        if (!g::engine_client->IsInGame() || entity_data::player_entry_data.empty())
            return;

        if (entity_data::locker.try_lock())
        {
            m_player_data.clear();
            std::ranges::copy(entity_data::player_entry_data.front().player_data, std::back_inserter(m_player_data));
            entity_data::locker.unlock();
        }
        
        float best_fov = 9999.f;
        
        for (const auto& data : m_player_data)
        {
            static int ticks = 0;

            QAngle viewangles;
            g::client->GetViewAngles(0, &viewangles);

            const auto& hitbox_ids = GetTargetHitboxes();
            if (hitbox_ids.empty())
                continue;

            if (!data.pawn || data.m_iHealth <= 0)
                continue;

            const auto& eye_pos = data.localplayer_pawn->GetEyePos();

            for (const auto& hitbox_id : hitbox_ids) //redo this
            {
                if (data.hitboxes.empty())
                {
                    printf("No hitboxes for player: %s\n", data.player_name);
                    continue;
                }

                for (auto& hitbox_data : data.hitboxes)
                {
                    if (hitbox_data.index != hitbox_id)
                        continue;

                    Vector hitbox_pos = hitbox_data.hitbox_pos;

                    QAngle target_angle = (hitbox_pos - eye_pos).to_qangle();
                    target_angle.clamp_normalize();

                    auto delta = target_angle - viewangles;
                    delta.clamp_normalize();

                    //float dist = hitbox_pos.dist_to(eye_pos); 
                    float dist = data.m_vecOrigin.dist_to(eye_pos);

                    float fov = distance_based_fov(delta, dist);

                    float delta_dist = viewangles.to_vector().dist_to(target_angle.to_vector());

                    auto viewangles_normalized = viewangles.to_vector();
                    viewangles_normalized.normalize();

                    auto target_angle_normalized = target_angle.to_vector();
                    target_angle_normalized.normalize();

                    float delta_vec_dist = viewangles_normalized.dist_to(target_angle_normalized);

                    if (fov < best_fov) 
                        best_fov = fov;

                    printf("[%s: %d]: fov: %.1f, best_fov: %.1f, dist: %.1f, delta_dist: %.1f, delta_vec_dist: %.1f\n", hitbox_data.entity_name, hitbox_data.index, fov, best_fov, dist, delta_dist, delta_vec_dist);

                    if (!GetAsyncKeyState(VK_LBUTTON))
                        continue;

                    if (best_fov > settings::visuals::aimbot_fov)
                        continue;

                    g::client->SetViewAngles(0, target_angle);
                }
            }
            ticks++;
        }
    }
}

bool hooks::clientmode_createmove::hooked(void* rcx, CUserCmd* cmd)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return original_fn(rcx, cmd);

    Aimbot::Aim(cmd);

    return original_fn(rcx, cmd);
}
