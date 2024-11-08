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

        static QAngle last_punch = { 0.f, 0.f, 0.f };

        //Add hitbox target priority?
        std::unordered_set<int> GetTargetHitboxes(const entity_data::player_data_t& data) 
        {
            static std::unordered_set<int> list;
            static int previous_hitbox_value = -1;

            int current_hitbox_value = settings::visuals::aimbot_hitbox;

            if (current_hitbox_value != previous_hitbox_value)
            {
                list.clear();

                if (current_hitbox_value & TARGET_HEAD)
                {
                    list.emplace(HITBOX_HEAD);
                    list.emplace(HITBOX_NECK);
                }

                if (current_hitbox_value & TARGET_CHEST)
                {
                    list.emplace(HITBOX_UPPER_CHEST);
                    list.emplace(HITBOX_LOWER_CHEST);
                    list.emplace(HITBOX_THORAX);
                    list.emplace(HITBOX_BELLY);
                }

                if (current_hitbox_value & TARGET_ARMS)
                {
                    list.emplace(HITBOX_LEFT_UPPER_ARM);
                    list.emplace(HITBOX_RIGHT_UPPER_ARM);
                }

                if (current_hitbox_value & TARGET_LEGS)
                {
                    list.emplace(HITBOX_LEFT_THIGH);
                    list.emplace(HITBOX_RIGHT_THIGH);
                    list.emplace(HITBOX_PELVIS);
                    list.emplace(HITBOX_LEFT_CALF);
                    list.emplace(HITBOX_RIGHT_CALF);
                }
            }

            if (current_hitbox_value & TARGET_NEAREST_IN_AIR && data.flags.test(PLAYER_IN_AIR))
            {
                list.clear();

                list.emplace(HITBOX_HEAD);
                list.emplace(HITBOX_NECK);
                list.emplace(HITBOX_UPPER_CHEST);
                list.emplace(HITBOX_LOWER_CHEST);
                list.emplace(HITBOX_THORAX);
                list.emplace(HITBOX_BELLY);
                list.emplace(HITBOX_LEFT_UPPER_ARM);
                list.emplace(HITBOX_RIGHT_UPPER_ARM);
                list.emplace(HITBOX_LEFT_THIGH);
                list.emplace(HITBOX_RIGHT_THIGH);
                list.emplace(HITBOX_PELVIS);
                list.emplace(HITBOX_LEFT_CALF);
                list.emplace(HITBOX_RIGHT_CALF);
            }
            else if (!data.flags.test(PLAYER_IN_AIR))
            {
                list.clear();

                if (list.empty() && previous_hitbox_value != -1)
                {
                    if (previous_hitbox_value & TARGET_HEAD)
                    {
                        list.emplace(HITBOX_HEAD);
                        list.emplace(HITBOX_NECK);
                    }

                    if (previous_hitbox_value & TARGET_CHEST)
                    {
                        list.emplace(HITBOX_UPPER_CHEST);
                        list.emplace(HITBOX_LOWER_CHEST);
                        list.emplace(HITBOX_THORAX);
                        list.emplace(HITBOX_BELLY);
                    }

                    if (previous_hitbox_value & TARGET_ARMS)
                    {
                        list.emplace(HITBOX_LEFT_UPPER_ARM);
                        list.emplace(HITBOX_RIGHT_UPPER_ARM);
                    }

                    if (previous_hitbox_value & TARGET_LEGS)
                    {
                        list.emplace(HITBOX_LEFT_THIGH);
                        list.emplace(HITBOX_RIGHT_THIGH);
                        list.emplace(HITBOX_PELVIS);
                        list.emplace(HITBOX_LEFT_CALF);
                        list.emplace(HITBOX_RIGHT_CALF);
                    }
                }
            }

            previous_hitbox_value = current_hitbox_value;

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

        Vector angle_to_pixel(const QAngle& aimAngle, const QAngle& currentAngle, int screenWidth, int screenHeight)
        {
            auto delta = aimAngle - currentAngle;
            delta.normalize_clamp();

            // Map delta angles to pixel coordinates on the screen
            float screenX = (-delta.yaw / 360.0f) * screenWidth; // Map yaw to [0, screenWidth]
            float screenY = (delta.pitch / 90.0f) * screenHeight; // Map pitch to [0, screenHeight]

            // Return the pixel coordinates as a Vector2
            return Vector(screenX, screenY, 0.f);
        }

        void move_mouse(int screenX, int screenY) 
        {
            mouse_event(MOUSEEVENTF_MOVE, screenX, screenY, 0, 0);
        }

        void smooth(float amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& out_angles)
        {
            float smoothing_factor = amount;

            QAngle delta = aim_angles - current_angles;
            delta.normalize_clamp();

            out_angles = current_angles + delta / smoothing_factor;
            out_angles.normalize_clamp();
        }
    
        void smooth_constant(float speed, const QAngle& current_angles, const QAngle& target_angles, QAngle& smoothed_angles) 
        {
            QAngle current_angle_normalized = current_angles;
            current_angle_normalized.normalize_clamp();

            QAngle target_angle_normalized = target_angles;
            target_angle_normalized.normalize_clamp();

            // Calculate the delta and normalize it to the shortest path
            QAngle delta = target_angle_normalized - current_angle_normalized;
            delta.normalize_clamp();

            // Define the maximum smoothing step based on speed
            float max_smooth_step = 10.0f; // Adjust this value for sensitivity
            float smooth_step = (max_smooth_step / (speed + 0.1f)) * (1.0f / 64.0f);
            
            // Start with the current angle
            smoothed_angles = current_angle_normalized;

            // Smooth the pitch
            if (std::fabs(delta.pitch) < smooth_step) {
                smoothed_angles.pitch = target_angle_normalized.pitch;
            }
            else {
                smoothed_angles.pitch += (delta.pitch > 0 ? smooth_step : -smooth_step);
            }

            // Smooth the yaw
            if (std::fabs(delta.yaw) < smooth_step) {
                smoothed_angles.yaw = target_angle_normalized.yaw;
            }
            else {
                smoothed_angles.yaw += (delta.yaw > 0 ? smooth_step : -smooth_step);
            }

            // Smooth the roll
            if (std::fabs(delta.roll) < smooth_step) {
                smoothed_angles.roll = target_angle_normalized.roll;
            }
            else {
                smoothed_angles.roll += (delta.roll > 0 ? smooth_step : -smooth_step);
            }

            // Normalize the final output angle
            smoothed_angles.normalize_clamp();
        }

        void rcs(CCSPlayerPawn* localpawn, const QAngle& viewangles, CUserCmd* cmd)
        {
            auto punch_cache = localpawn->m_aimPunchCache();
            if (punch_cache.Count() <= 0 || punch_cache.Count() >= 0xFFFF)
                return;

            // Get the latest punch angle
            QAngle current_punch = punch_cache[punch_cache.Count() - 1];
            current_punch.pitch *= settings::visuals::pitch;
            current_punch.yaw *= settings::visuals::yaw;
            current_punch.normalize_clamp();

            if (localpawn->m_iShotsFired() > 1 && g::input_system->IsButtonDown(ButtonCode::MouseLeft)) // Apply while shooting
            {
                QAngle recoil_delta = current_punch - last_punch; // Determine the change in punch angle
                recoil_delta.pitch *= settings::visuals::pitch;
                recoil_delta.yaw *= settings::visuals::yaw;
                recoil_delta.normalize_clamp();

                QAngle compensated_angle = viewangles - recoil_delta; // Adjust based on the recoil
                compensated_angle.normalize_clamp();

                QAngle output;
                smooth(1.1f, viewangles, compensated_angle, output);
                output.normalize_clamp();

                g::client->SetViewAngles(0, output);

                last_punch = current_punch;
            }
            else
            {
                last_punch = { 0.0f, 0.0f, 0.f };
            }
        }
        
        const char* hitbox_index_to_name(int index) 
        {
            switch (index) 
            {
                case HITBOX_HEAD:            
                    return "Head";
                case HITBOX_NECK:            
                    return "Neck";
                case HITBOX_PELVIS:          
                    return "Pelvis";
                case HITBOX_BELLY:           
                    return "Belly";
                case HITBOX_THORAX:          
                    return "Thorax";
                case HITBOX_LOWER_CHEST:     
                    return "Lower Chest";
                case HITBOX_UPPER_CHEST:     
                    return "Upper Chest";
                case HITBOX_RIGHT_THIGH:      
                    return "Right Thigh";
                case HITBOX_LEFT_THIGH:       
                    return "Left Thigh";
                case HITBOX_RIGHT_CALF:       
                    return "Right Calf";
                case HITBOX_LEFT_CALF:        
                    return "Left Calf";
                case HITBOX_RIGHT_FOOT:       
                    return "Right Foot";
                case HITBOX_LEFT_FOOT:        
                    return "Left Foot";
                case HITBOX_RIGHT_HAND:       
                    return "Right Hand";
                case HITBOX_LEFT_HAND:        
                    return "Left Hand";
                case HITBOX_RIGHT_UPPER_ARM:  
                    return "Right Upper Arm";
                case HITBOX_RIGHT_FOREARM:    
                    return "Right Forearm";
                case HITBOX_LEFT_UPPER_ARM:   
                    return "Left Upper Arm";
                case HITBOX_LEFT_FOREARM:     
                    return "Left Forearm";
            }
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

            for (auto& data : m_player_data)
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

                    const auto& hitbox_ids = GetTargetHitboxes(data);
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
                    if (fov < best_fov)
                    {
                        best_fov = fov;
                        best_angle = target_angle;
                    }

                    /*if (!cmd->IsButtonPressed(IN_ATTACK))
                        continue;*/

                    const auto& active_wpn_handle = localpawn->m_pWeaponServices()->m_hActiveWeapon();
                    if (!active_wpn_handle.IsValid())
                        continue;

                    auto active_wpn = reinterpret_cast<CBasePlayerWeapon*>(g::entity_system->GetEntityFromHandle(active_wpn_handle));
                    if (!active_wpn)
                        continue;

                    if (!(GetAsyncKeyState(VK_LBUTTON)))
                        continue;

                    if (active_wpn->m_iItemDefinitionIndex() == WEAPON_AWP && !localpawn->m_bIsScoped())
                        continue;

                    int next_attack_tick = active_wpn->m_nNextPrimaryAttackTick().m_Value(); //This next shot attack checker works poorly on awp - needs fix!
                    if (next_attack_tick <= localplayer->m_nTickBase() && active_wpn->m_iItemDefinitionIndex() != WEAPON_AWP)
                        continue;

                    //printf("[%s: %s]: fov: %.1f, best_fov: %.1f, dist: %.1f\n", data.m_szPlayerName, hitbox_index_to_name(hitbox_data->index), fov, best_fov, distance);

                    if (best_fov > settings::visuals::aimbot_fov)
                        continue;

                    QAngle output;
                    if (settings::visuals::smooth_mode == 0)
                        smooth(settings::visuals::smooth, viewangles, best_angle, output);
                    else if (settings::visuals::smooth_mode == 1)
                        smooth_constant(settings::visuals::smooth, viewangles, best_angle, output);

                    g::client->SetViewAngles(0, output);
                }
            }

            rcs(localpawn, viewangles, cmd);
        }
    }
}