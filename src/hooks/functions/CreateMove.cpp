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
        static std::unordered_set<int> list = { HITBOX_HEAD, HITBOX_NECK, HITBOX_LOWER_CHEST, HITBOX_UPPER_CHEST, HITBOX_BELLY, HITBOX_THORAX };

        return list;
    }

    void smooth(const float& amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& out_angles)
    {
        QAngle clamped_aim_angles = aim_angles;
        clamped_aim_angles.clamp_normalize();

        auto corrected_amount = amount;
        const float tickrate = 1.0f / 0.015625f; // interval_per_tick for 64 tick
        corrected_amount = tickrate * amount / 64.0f;

        if (corrected_amount <= 1.1f)
        {
            out_angles = clamped_aim_angles;
            return;
        }

        Vector aim_vector = clamped_aim_angles.to_vector();
        Vector current_vector = current_angles.to_vector();

        Vector delta = aim_vector - current_vector;
        Vector smoothed = current_vector + delta / corrected_amount;

        out_angles = smoothed.to_qangle();
        out_angles.clamp_normalize();
    }

    void smooth_constant(const float& speed, const QAngle& current_angles, const QAngle& target_angles, QAngle& smoothed_angles)
    {
        QAngle delta;
        delta.pitch = target_angles.pitch - current_angles.pitch;
        delta.yaw = target_angles.yaw - current_angles.yaw;
        delta.clamp_normalize();

        float step = speed * 0.015625f;

        float length = sqrtf(delta.pitch * delta.pitch + delta.yaw * delta.yaw);

        if (length > step)
        {
            delta.normalize(); 
            delta *= step;
        }

        smoothed_angles.pitch = current_angles.pitch + delta.pitch;
        smoothed_angles.yaw = current_angles.yaw + delta.yaw;
        smoothed_angles.clamp_normalize();
    }

    void recoil(CCSPlayerPawn* localpawn, QAngle& viewangles)
    {
        static QAngle old_angle = { 0, 0, 0 };

        auto& punch_cache = localpawn->m_aimPunchCache();

        if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
            return;

        auto punch_angle = punch_cache[punch_cache.Count() - 1];
        
        if (localpawn->m_iShotsFired() >= 1)
        {
            QAngle new_punch = punch_angle;
            new_punch.pitch *= 0.2f;
            new_punch.yaw *= 2.f;

            /*new_punch.pitch -= old_angle.pitch;
            new_punch.yaw -= old_angle.yaw;*/

            viewangles.pitch -= (punch_angle.pitch * 0.2f);
            viewangles.yaw -= (punch_angle.yaw);
            viewangles.clamp_normalize();

            auto recoil_angle = viewangles;
            recoil_angle.clamp_normalize();

            smooth_constant(settings::visuals::const_smooth ? 24 : 12, viewangles, recoil_angle, recoil_angle);

            if ((recoil_angle.pitch != 0.f || recoil_angle.yaw != 0.f) && GetAsyncKeyState(VK_LBUTTON))
            {
                g::client->SetViewAngles(0, recoil_angle);
            }
        }
        old_angle = punch_angle;
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

        /*auto& punch_cache = localpawn->m_aimPunchCache();

        if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
            return;

        auto punch_angle = punch_cache[punch_cache.Count() - 1];

        viewangles.pitch -= (punch_angle.pitch * 0.2f); //This works, but its not right
        viewangles.yaw -= (punch_angle.yaw);
        viewangles.clamp_normalize();*/

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

                //recoil(localpawn, viewangles);

                if (!GetAsyncKeyState(VK_LBUTTON))
                    continue;

                if (best_fov > settings::visuals::aimbot_fov)
                    continue;

                if (!settings::visuals::const_smooth)
                    smooth(settings::visuals::smooth, viewangles, best_angle, best_angle);
                else smooth_constant(settings::visuals::smooth, viewangles, best_angle, best_angle);

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
