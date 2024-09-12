#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../sdk/math/math.h"
#include "../../sdk/localplayer.h"
#include "../../settings/settings.h"
#include <unordered_set>

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
    std::list<entity_data::player_data_t> m_player_data;

	std::unordered_set<int>& GetTargetHitboxes()
    {
        static std::unordered_set<int> list = { HITBOX_HEAD, HITBOX_LOWER_CHEST, HITBOX_UPPER_CHEST };

        return list;
    }

    void Aim(CUserCmd* cmd)
    {
        if (!g::engine_client->IsInGame())
            return;

        if (entity_data::locker.try_lock())
        {
            m_player_data.clear();

            if (!entity_data::player_entry_data.empty())
                std::ranges::copy(entity_data::player_entry_data.front().player_data, std::back_inserter(m_player_data));

            entity_data::locker.unlock();
        }
        
        QAngle viewangles;
        g::client->GetViewAngles(0, &viewangles);

        float best_fov = 9999.f;
        QAngle best_angle;

        for (const auto& data : m_player_data)
        {
            if (!data.pawn || data.m_iHealth <= 0)
                continue;

            if (data.hitboxes.empty())
                continue;

            for (auto& hitbox_data : data.hitboxes)
            {
                const auto& hitbox_ids = GetTargetHitboxes();
                if (hitbox_ids.empty())
                    continue;

                if (hitbox_ids.find(hitbox_data.index) == hitbox_ids.end())
                    continue;

                const Vector& hitbox_pos = hitbox_data.hitbox_pos;

                auto eye_pos = data.localplayer_pawn->GetEyePos();

                Vector delta_target_angle = (hitbox_pos - eye_pos);

                QAngle target_angle = delta_target_angle.to_qangle();
                target_angle.clamp_normalize();

                auto delta = target_angle - viewangles;
                delta.clamp_normalize();

                //float dist = hitbox_pos.dist_to(eye_pos); //returns bullshit distance
                float dist = data.m_vecOrigin.dist_to(eye_pos);

                //float fov = distance_based_fov(delta, dist);

                float fov = std::hypotf(delta.pitch, delta.yaw);

                //if (fov < best_fov)
                //{
                //    best_fov = fov;
                //    best_angle = target_angle; //best angle not applying to other enemies?
                //}

                printf("[%s: %d]: fov: %.1f, best_fov: %.1f, dist: %.1f\n", hitbox_data.entity_name, hitbox_data.index, fov, best_fov, dist);

                if (!GetAsyncKeyState(VK_LBUTTON))
                    continue;

                if (fov > settings::visuals::aimbot_fov)
                    continue;

                g::client->SetViewAngles(0, target_angle);
            }
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
