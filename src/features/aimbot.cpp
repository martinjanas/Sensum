#include <unordered_set>
#include <mutex>
#include <algorithm>

#include "features.h"
#include "../settings/settings.h"
#include "../sdk/math/math.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/sdk.h"
#include "../sdk/helpers/utils.h"
#include "../sdk/helpers/Timer.h"

namespace features
{
    namespace aimbot
    {
        std::list<entity_data::player_data_t> m_player_data;

        static QAngle last_punch = { 0.f, 0.f, 0.f };
        settings::aimbot::weapon_config_t weapon_config;

        //Add hitbox target priority?
        std::unordered_set<int> GetTargetHitboxes(const entity_data::player_data_t& data) //takes 1300 ns
        {
            static std::unordered_set<int> list;
            static int previous_hitbox_value = -1;

            int current_hitbox_value = weapon_config.hitboxes;

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

        void rcs(CCSPlayerPawn* localpawn, QAngle& viewangles, CUserCmd* cmd)
        {
            const auto& punch_cache = localpawn->m_aimPunchCache();
            if (punch_cache.Count() <= 1 || punch_cache.Count() >= 0xFFFF)
                return;

            QAngle current_punch = punch_cache[punch_cache.Count() - 1];
            current_punch.pitch *= (weapon_config.recoil.pitch * 1.25f);
            current_punch.yaw *= (weapon_config.recoil.yaw * 1.25f);
            current_punch.normalize_clamp();

            if (localpawn->m_iShotsFired() > 1 && g::input_system->IsButtonDown(ButtonCode::MouseLeft))
            {
                QAngle recoil_delta = current_punch - last_punch;
                recoil_delta.normalize_clamp();

                QAngle compensated_angle = viewangles - recoil_delta;
                compensated_angle.normalize_clamp();

                QAngle output = viewangles.linear_smooth(compensated_angle, 1.23f);
                output.normalize_clamp();

                g::client->SetViewAngles(output);
            }
            else
            {
                last_punch = { 0.0f, 0.0f, 0.f };
            }
            last_punch = current_punch;
        }
        
        bool is_weapon_valid(CBasePlayerWeapon* weapon)
        {
            return !(weapon->IsKnife() || weapon->IsC4() || weapon->IsTaser() || weapon->IsGrenade() || weapon->IsHealthshot());
        }

        void handle(CUserCmd* cmd)
        {
            if (!g::engine_client->IsInGame())
                return;

            std::shared_lock<std::shared_mutex> lock(entity_data::player_locker);

            m_player_data.clear();
            if (!entity_data::player_entry_data.empty())
                std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_player_data));

            QAngle viewangles;
            g::client->GetViewAngles(&viewangles);

            float best_fov = 9999.f;
            QAngle best_angle;

            CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
            if (!localplayer)
                return;

            CCSPlayerPawn* localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPlayerPawn());  //localplayer->m_hPlayerPawn().Get<CCSPlayerPawn*>();
            if (!localpawn)
                return;

            const auto& active_wpn_handle = localpawn->m_pWeaponServices()->m_hActiveWeapon();
            if (!active_wpn_handle.IsValid())
                return;

            auto active_wpn = reinterpret_cast<CBasePlayerWeapon*>(g::entity_system->GetEntityFromHandle(active_wpn_handle));
            if (!active_wpn)
                return;

            const auto& wep_data = active_wpn->m_pWpnData();
            if (!wep_data)
                return;

            int index = active_wpn->m_iItemDefinitionIndex();

            weapon_config = settings::aimbot::weapon_configs[index];

            const auto& eye_pos = localpawn->GetEyePos();
            for (auto& data : m_player_data)
            {
                if (!data.m_PlayerPawn || data.m_iHealth <= 0)
                    continue;

                if (data.flags.test(PLAYER_IN_SMOKE) || !data.flags.test(PLAYER_VISIBLE))
                    continue;

                if (data.hitboxes.empty())
                    continue;

                const auto& hitbox_ids = GetTargetHitboxes(data);
                if (hitbox_ids.empty())
                    continue;

                for (int i = 0; i < data.hitboxes.size(); i++)
                {
                    auto* hitbox_data = &data.hitboxes[i];
                    if (!hitbox_data)
                        continue;

                    if (hitbox_ids.find(hitbox_data->index) == hitbox_ids.end())
                        continue;

                    //reset the target when not visible, etc... ?
                    if (!hitbox_data->visible)
                        continue;

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

                    if (!(cmd->nButtons.nValue & IN_ATTACK))
                        continue;

                    if (!is_weapon_valid(active_wpn))
                        continue;

                    if (active_wpn->IsSniper() && !localpawn->m_bIsScoped())
                        continue;

                    if (active_wpn->m_bInReload())
                        continue;

                    //TODO: Implement proper next_attack check using more stuff?
                    int next_attack_tick = active_wpn->m_nNextPrimaryAttackTick().m_Value(); //This next shot attack checker works poorly on awp - needs fix!
                    if (next_attack_tick <= localplayer->m_nTickBase() && !active_wpn->IsSniper()) //temp fix
                        continue;

                    //printf("[%s: %s]: fov: %.1f, best_fov: %.1f, dist: %.1f\n", data.m_szPlayerName, utils::hitbox_index_to_name(hitbox_data->index), fov, best_fov, distance);

                    if (best_fov > weapon_config.fov)
                        continue;

                    weapon_config.smooth_time = std::clamp(weapon_config.smooth_time, 0.0f, 1.0f);

                    static Timer timer(weapon_config.smooth_time);
                    timer.set_duration(weapon_config.smooth_time);
                    timer.has_finished();

                    float elapsed_time = timer.get_time_remaining();
                    float t = elapsed_time / weapon_config.smooth_time;
                    t = std::clamp(t, 0.0f, 1.0f);

                    QAngle rcs_delta = best_angle - viewangles;
                    rcs_delta.normalize_clamp();
                    bool is_spraying = rcs_delta.length() > 0.01f && localpawn->m_iShotsFired() > 1;

                    QAngle output;
                    if (weapon_config.smooth_mode == 0)
                        output = viewangles.linear_smooth(best_angle, weapon_config.smooth);
                    else if (weapon_config.smooth_mode == 1)
                        output = viewangles.const_smooth(best_angle, weapon_config.smooth);
                    else if (weapon_config.smooth_mode == 2)
                        output = viewangles.lerp(best_angle, t);

                    output.normalize_clamp();

                    if (t >= 1.0f)
                        timer.reset();

                    g::client->SetViewAngles(output);
                }
            }

            rcs(localpawn, viewangles, cmd);
        }
    }
}