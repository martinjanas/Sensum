#include <unordered_set>
#include <mutex>
#include <algorithm>

#include "features.h"
#include "../settings/settings.h"
#include "../sdk/math/math.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/sdk.h"


namespace features
{
    namespace aimbot
    {
        std::list<entity_data::player_data_t> m_player_data;

        static QAngle old_punch = { 0.f, 0.f, 0.f };
        static QAngle current_punch = { 0.f, 0.f, 0.f };
        static bool was_firing_last_frame = false;

        std::unordered_set<int>& GetTargetHitboxes() //Add hitbox target priority?
        {
            static std::unordered_set<int> list = { HITBOX_HEAD, HITBOX_NECK, HITBOX_LOWER_CHEST, HITBOX_UPPER_CHEST, HITBOX_BELLY, HITBOX_THORAX };

            return list;
        }

        float distance_based_fov(const QAngle& delta, const float& distance)
        {
            float pitch_diff = std::sinf(fabsf(delta.pitch) * math::deg2rad) * distance;
            float yaw_diff = std::sinf(fabsf(delta.yaw) * math::deg2rad) * distance;

            float fov = std::hypotf(pitch_diff, yaw_diff);

            fov = std::clamp<float>(fov, 0.f, 180.f);

            return fov;
        }

        void smooth(float amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& out_angles)
        {
            float smoothing_factor = amount;

            QAngle delta = aim_angles - current_angles;
            delta.normalize_clamp();

            out_angles = current_angles + delta / smoothing_factor;
            out_angles.normalize_clamp();
        }

        //broken, not constant at all
        void smooth_constant(const float& speed, const QAngle& current_angles, const QAngle& target_angles, QAngle& smoothed_angles)
        {
            QAngle delta = target_angles - current_angles;
            delta.normalize_clamp();

            constexpr float tick_interval = 1.f / 64.f;
            float step = speed * tick_interval;

            float length = sqrtf(delta.pitch * delta.pitch + delta.yaw * delta.yaw);

            if (length > step)
            {
                delta.normalize();
                delta *= step;
            }

            smoothed_angles = current_angles + delta;
            smoothed_angles.normalize_clamp();
        }

        void smooth_exponential(const float& amount, const QAngle& current_angles, const QAngle& target_angles, QAngle& smoothed_angles)
        {
            //amount - ideal value: 1.0 - 1.3

            // Clamp and normalize target angles to prevent issues with extreme values
            QAngle clamped_target_angles = target_angles;
            clamped_target_angles.normalize_clamp();

            // Compute the corrected amount for smoothing based on the tickrate
            const float tickrate = 1.0f / 0.015625f; // interval_per_tick for 64 tick
            float corrected_amount = std::max(amount, 1.1f) * tickrate / 64.0f;

            // Convert angles to vectors for easier manipulation
            Vector current_vector = current_angles.to_vector();
            Vector target_vector = clamped_target_angles.to_vector();

            // Calculate the delta vector between the current and target vectors
            Vector delta = target_vector - current_vector;

            // Apply exponential smoothing
            Vector smoothed_vector = current_vector + delta / corrected_amount;

            // Convert back to angles and clamp/normalize the result
            smoothed_angles = smoothed_vector.to_qangle();
            smoothed_angles.normalize_clamp();
        }
        
        void recoil(CCSPlayerPawn* localpawn, const QAngle& viewangles)
        {
            // Get a reference to the punch angle cache
            auto& punch_cache = localpawn->m_aimPunchCache();

            // Ensure the punch cache is valid
            if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
                return;

            QAngle cp = punch_cache[punch_cache.Count()];
            QAngle cpm1 = punch_cache[punch_cache.Count() - 1];
            QAngle cpm2 = punch_cache[punch_cache.Count() - 2];

            int cp_count = punch_cache.Count();
            int cp1_count = punch_cache.Count() - 1;
            int cp2_count = punch_cache.Count() - 2;

            g_Console->println("cpc: %d, cp: %.1f, %.1f", cp_count, cp.pitch, cp.yaw);
            g_Console->println("cp1c: %d, cp1: %.1f, %.1f", cp1_count, cpm1.pitch, cpm1.yaw);
            g_Console->println("cp2c: %d, cp2: %.1f, %.1f", cp2_count, cpm2.pitch, cpm2.yaw);

            //// Get the most recent punch angle from the cache
            //QAngle cp = punch_cache[punch_cache.Count() - 1]; // Most recent angle at index 0

            //QAngle delta = viewangles - cp;
            //delta.normalize_clamp();

            ////.Count() is 129 when not shooting, once shooting it starts at 0 and goes up quickly up to 129
            //g_Console->println("size: %d", punch_cache.Count());
            /*g_Console->println("v: %.1f, %.1f", viewangles.pitch, viewangles.yaw);
            g_Console->println("cp: %.1f, %.1f", cp.pitch, cp.yaw);
            g_Console->println("delta: %.1f, %.1f", delta.pitch, delta.yaw);*/

            if (g::input_system->IsButtonDown(ButtonCode::MouseLeft))
            {
                QAngle delta = viewangles - cp;

                smooth(settings::visuals::recoil_smooth, viewangles, delta, delta);

                g::client->SetViewAngles(0, delta);
            }
        }

        //Standlone RCS:
        //void recoil(CCSPlayerPawn* localpawn, const QAngle& viewangles, CUserCmd* cmd)
        //{
        //    auto& punch_cache = localpawn->m_aimPunchCache();
        //    if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
        //        return;

        //    if (g::input_system->IsButtonDown(ButtonCode::MouseLeft))
        //    {
        //        current_punch = punch_cache[punch_cache.Count() - 1];
        //        current_punch.pitch *= settings::visuals::pitch;
        //        current_punch.yaw *= settings::visuals::yaw;
        //        
        //        QAngle compensated_angle = viewangles + (old_punch - current_punch);
        //        compensated_angle.normalize_clamp();

        //        //smooth(settings::visuals::recoil_smooth, viewangles, compensated_angle, compensated_angle);

        //        g_Console->println("cp: %.1f, %.1f", current_punch.pitch, current_punch.yaw);
        //        g_Console->println("op: %.1f, %.1f", old_punch.pitch, old_punch.yaw);
        //        g_Console->println("ca: %.1f, %.1f", compensated_angle.pitch, compensated_angle.yaw);

        //        /*if (compensated_angle.pitch > 12.f)
        //            return;*/

        //        g::client->SetViewAngles(0, compensated_angle);

        //        old_punch = punch_cache[punch_cache.Count() - 2];
        //    }

        //    old_punch = current_punch;
        //}

        void recoil(CCSPlayerPawn* localpawn, const QAngle& viewangles, CUserCmd* cmd)
        {
            auto& punch_cache = localpawn->m_aimPunchCache();
            if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
                return;

            if (g::input_system->IsButtonDown(ButtonCode::MouseLeft))
            {
                current_punch = punch_cache[punch_cache.Count() - 1];
                current_punch.pitch *= settings::visuals::pitch;
                current_punch.yaw *= settings::visuals::yaw;

                QAngle compensated_angle = viewangles + (old_punch - current_punch);
                compensated_angle.normalize_clamp();

                //smooth(settings::visuals::recoil_smooth, viewangles, compensated_angle, compensated_angle);

                g_Console->println("cp: %.1f, %.1f", current_punch.pitch, current_punch.yaw);
                g_Console->println("op: %.1f, %.1f", old_punch.pitch, old_punch.yaw);
                g_Console->println("ca: %.1f, %.1f", compensated_angle.pitch, compensated_angle.yaw);

                /*if (compensated_angle.pitch > 12.f)
                    return;*/

                g::client->SetViewAngles(0, compensated_angle);

                old_punch = punch_cache[punch_cache.Count() - 2];
            }

            old_punch = current_punch;
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
                    std::ranges::copy(entity_data::player_entry_data.back().player_data, std::back_inserter(m_player_data));
            }

            QAngle viewangles;
            g::client->GetViewAngles(0, &viewangles);

            float best_fov = 9999.f;
            QAngle best_angle;

            CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
            if (!localplayer)
                return;

            CCSPlayerPawn* localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPlayerPawn());  //localplayer->m_hPlayerPawn().Get<CCSPlayerPawn*>();
            if (!localpawn)
                return;

            const auto& eye_pos = localpawn->GetEyePos();

            //auto& punch_cache = localpawn->m_aimPunchCache();

            //if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
            //    return;

            //auto punch_angle = punch_cache[punch_cache.Count() - 1];

            //auto recoil_angle = localpawn->m_iShotsFired() > 1 ? punch_angle : QAngle();
            //recoil_angle.normalize_clamp();

            //viewangles -= recoil_angle; //This works, but its not allright
            //viewangles.normalize_clamp();

            for (const auto& data : m_player_data)
            {
                if (!data.m_PlayerPawn || data.m_iHealth <= 0)
                    continue;

                if (data.flags.test(PLAYER_IN_SMOKE))
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

                    if (!hitbox_data->visible)
                        continue;

                    //reset the target when not visible, etc...

                    QAngle target_angle = (hitbox_data->hitbox_pos - eye_pos).to_qangle();
                    target_angle.normalize_clamp();

                    auto delta = target_angle - viewangles;
                    delta.normalize_clamp();

                    float distance = hitbox_data->hitbox_pos.dist_to(eye_pos);
                    //float distance = data.m_vecOrigin.dist_to(eye_pos);

                    float fov = distance_based_fov(delta, distance);

                    //float fov = std::hypotf(delta.pitch, delta.yaw);

                    if (fov < best_fov)
                    {
                        best_fov = fov;
                        best_angle = target_angle;
                    }

                    /*if (!cmd->IsButtonPressed(IN_ATTACK))
                        continue;*/

                    if (!(GetAsyncKeyState(VK_LBUTTON)))
                        continue;

                    //printf("[%s: %d]: fov: %.1f, best_fov: %.1f, dist: %.1f\n", data.m_szPlayerName, hitbox_data->index, fov, best_fov, distance);

                    if (best_fov > settings::visuals::aimbot_fov)
                        continue;

                    /*if (!settings::visuals::const_smooth)
                        smooth(settings::visuals::smooth, viewangles, best_angle, best_angle);
                    else smooth_constant(settings::visuals::smooth, viewangles, best_angle, best_angle);*/

                    smooth(settings::visuals::smooth, viewangles, best_angle, best_angle);

                    g::client->SetViewAngles(0, best_angle);
                }
            }

            recoil(localpawn, viewangles, cmd); //This keeps overwriting my angles and thus aimbot is not locking on
        }
    }
}