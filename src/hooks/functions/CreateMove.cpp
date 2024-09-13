#include "../hooks.h"
#include "../../sdk/sdk.h"
#include "../../sdk/helpers/entity_data.h"
#include "../../sdk/math/math.h"
#include "../../sdk/localplayer.h"
#include "../../settings/settings.h"
#include <unordered_set>
#include <mutex>

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

    void smooth(const float& amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& angles)
    {
        angles = aim_angles;
        angles.clamp_normalize();

        auto corrected_amount = amount;
        auto tickrate = 1.0f / 0.015625; //interval_per_tick on 64 tick

        corrected_amount = tickrate * amount / 64.f;

        if (corrected_amount < 1.1f)
            return;

        Vector aim_vector = aim_angles.to_vector();
        Vector current_vector = current_angles.to_vector();
        
        auto delta = aim_vector - current_vector;
       
        auto smoothed = current_vector + delta / corrected_amount;

        angles = smoothed.to_qangle();
        angles.clamp_normalize();
    }

    void handle(CUserCmd* cmd)
    {
        if (!g::engine_client->IsInGame())
            return;
  
        if (entity_data::locker.try_lock())
        {
            std::lock_guard<std::mutex> lock(entity_data::locker, std::adopt_lock);

            m_player_data.clear();

            if (!entity_data::player_entry_data.empty())
                std::ranges::copy(entity_data::player_entry_data.front().player_data, std::back_inserter(m_player_data));
        }

        QAngle viewangles;
        g::client->GetViewAngles(0, &viewangles);
        
        float best_fov = 9999.f;
        QAngle best_angle;

        CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();

        if (!localplayer)
            return;

        CCSPlayerPawn* localpawn = localplayer->m_hPlayerPawn().Get<CCSPlayerPawn*>();

        if (!localpawn)
            return;

        const auto& eye_pos = localpawn->GetEyePos();

        for (const auto& data : m_player_data)
        {
            if (!data.m_PlayerPawn || data.m_iHealth <= 0)
                continue;

            if (data.hitboxes.empty())
                continue;

            for (int i = 0; i < data.hitboxes.size(); i++)
            {
                auto* hitbox_data = &data.hitboxes[i];

                if (!hitbox_data)
                    continue;

                const auto& hitbox_ids = GetTargetHitboxes();
                if (hitbox_ids.empty())
                    continue;

                if (hitbox_ids.find(hitbox_data->index) == hitbox_ids.end())
                    continue;

                QAngle target_angle = (hitbox_data->hitbox_pos - eye_pos).to_qangle();
                target_angle.clamp_normalize();

                auto delta = target_angle - viewangles;
                delta.clamp_normalize();

                float distance = hitbox_data->hitbox_pos.dist_to(eye_pos);
                //float distance = data.m_vecOrigin.dist_to(eye_pos);

                float fov = distance_based_fov(delta, distance);

                if (fov < best_fov)
                {
                    best_fov = fov;
                    best_angle = target_angle;
                }

                //printf("[%s: %d]: fov: %.1f, best_fov: %.1f, dist: %.1f\n", hitbox_data->entity_name, hitbox_data->index, fov, best_fov, distance);

                if (!GetAsyncKeyState(VK_LBUTTON))
                    continue;

                if (best_fov > settings::visuals::aimbot_fov)
                    continue;

                smooth(settings::visuals::smooth, viewangles, best_angle, viewangles);

                g::client->SetViewAngles(0, best_angle);
            }
        }
    }
}

bool hooks::clientmode_createmove::hooked(void* rcx, CUserCmd* cmd)
{
    if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
        return original_fn(rcx, cmd);

    //entity_data::fetch_player_data(cmd);

    Aimbot::handle(cmd);

    return original_fn(rcx, cmd);
}
