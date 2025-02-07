#include <unordered_set>
#include <mutex>
#include <algorithm>

#include "features.h"
#include "../settings/settings.h"
#include "../sdk/math/math.h"
#include "../sdk/helpers/entity_data.h"
#include "../sdk/sdk.h"
#include "../sdk/helpers/Timer.h"
#include "../sdk/helpers/utils.h"

#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / 0.015625 ) )
#define TICKS_TO_TIME(t) ( 0.015625 * (t) )

namespace features::aimbot
{
    target_info_t* current_target = nullptr;
    CBasePlayerWeapon* active_wpn = nullptr;
    settings::aimbot::weapon_config_t weapon_config;  
    static QAngle last_punch;
    
    std::list<entity_data::player_data_t> m_player_data;
    
    //Add hitbox target priority?
    std::unordered_set<int> GetTargetHitboxes(const entity_data::player_data_t& player_data) //takes 1300 ns
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

        if (current_hitbox_value & TARGET_NEAREST_IN_AIR && player_data.flags.test(PLAYER_IN_AIR))
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
        else if (!player_data.flags.test(PLAYER_IN_AIR))
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
        
    std::vector<target_info_t> fetch_targets(const Vector& eye_pos, const QAngle& viewangles)
    {
        std::vector<target_info_t> targets;

        float best_fov = 9999.f;
        QAngle best_angle;
        
        for (auto& player_data : m_player_data)
        {
            if (!player_data.m_PlayerPawn || player_data.m_iHealth <= 0)
                continue;

            if (!player_data.flags.test(PLAYER_VISIBLE) || player_data.flags.test(PLAYER_IN_SMOKE))
                continue;
            
            const auto& hitbox_ids = GetTargetHitboxes(player_data);
            if (hitbox_ids.empty())
                continue;
                
            for (auto& hitbox_data : player_data.hitboxes)
            {
                if (!hitbox_ids.contains(hitbox_data.index) || !hitbox_data.visible)
                    continue;
                    
                QAngle target_angle = (hitbox_data.hitbox_pos - eye_pos).to_qangle();
                target_angle.normalize_clamp();
                    
                auto delta = target_angle - viewangles;
                delta.normalize_clamp();

                float distance = hitbox_data.hitbox_pos.dist_to(eye_pos);

                float fov = math::distance_based_fov(delta, distance);
                if (fov < best_fov)
                {
                    best_fov = fov;
                    best_angle = target_angle;
                }

                targets.emplace_back(player_data.m_PlayerPawn, &player_data, fov, player_data.m_vecOrigin.dist_to(eye_pos), player_data.sim_time, player_data.flags.test(PLAYER_IN_AIR), best_angle, player_data.m_szPlayerName);
            }
        }
        return targets;
    }
        
    void rcs(CCSPlayerPawn* localpawn, QAngle& viewangles, CUserCmd* cmd, bool in_fov)
    {
        if (weapon_config.recoil.fov_based && !in_fov)
            return;
                
        const auto& punch_cache = localpawn->m_aimPunchCache();
        if (punch_cache.Count() <= 1 || punch_cache.Count() >= 0xFFFF)
            return;

        QAngle current_punch = punch_cache[punch_cache.Count() - 1];
        current_punch.pitch *= (weapon_config.recoil.pitch);
        current_punch.yaw *= (weapon_config.recoil.yaw);
        current_punch.normalize_clamp();
            
        //g::input_system->IsButtonDown(ButtonCode::MouseLeft)
        if (localpawn->m_iShotsFired() > 1 && cmd->nButtons.nValue & IN_ATTACK)
        {
            QAngle recoil_delta = current_punch - last_punch;
            recoil_delta.normalize_clamp();

            QAngle compensated_angle = viewangles - recoil_delta;
            compensated_angle.normalize_clamp();

            QAngle output = viewangles.linear_smooth(compensated_angle, 1.1f);
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

        std::lock_guard<std::mutex> lock(entity_data::player_locker);
        
        m_player_data.clear();
        if (!entity_data::player_entry_data.empty())
            std::copy(entity_data::player_entry_data.front().player_data.begin(), entity_data::player_entry_data.front().player_data.end(), std::back_inserter(m_player_data));
        
        CCSPlayerController* localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
        if (!localplayer)
            return;

        CCSPlayerPawn* localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(localplayer->m_hPlayerPawn());  //localplayer->m_hPlayerPawn().Get<CCSPlayerPawn*>();
        if (!localpawn)
            return;
            
        QAngle viewangles;
        g::client->GetViewAngles(&viewangles);

        const auto eye_pos = localpawn->GetEyePos();
            
        auto targets = fetch_targets(eye_pos, viewangles);
 
        std::sort(targets.begin(), targets.end());
        
        const auto& active_wpn_handle = localpawn->m_pWeaponServices()->m_hActiveWeapon();
        if (!active_wpn_handle.IsValid())
            return;

        auto active_wpn = reinterpret_cast<CBasePlayerWeapon*>(g::entity_system->GetEntityFromHandle(active_wpn_handle));
        if (!active_wpn)
            return;

        const auto& wep_data = active_wpn->m_pWpnData();
        if (!wep_data)
            return;
        
        weapon_config = settings::aimbot::weapon_configs[active_wpn->m_iItemDefinitionIndex()];
        
        bool in_fov = false;
        for (auto& target : targets)
        {
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

            //g_Console->println("[ %s ]: fov: %.1f, dist: %.1f", target.name, target.fov, target.distance);
            
            if (!(cmd->nButtons.nValue & IN_ATTACK))
                continue;

            if (target.fov > weapon_config.fov)
                continue;
                
            in_fov = true;
                
            weapon_config.smooth_time = std::clamp(weapon_config.smooth_time, 0.0f, 1.0f);
                    
            static Timer timer(weapon_config.smooth_time);
            timer.set_duration(weapon_config.smooth_time);
            timer.has_finished();
                
            float elapsed_time = timer.get_time_remaining();
            float t = elapsed_time / weapon_config.smooth_time;
            t = std::clamp(t, 0.0f, 1.0f);

            //g_Console->println("t: %.2f, elapsed_time: %.2f", t, elapsed_time);

            QAngle rcs_delta = target.target_angle - viewangles;
            rcs_delta.normalize_clamp();
            bool is_spraying = rcs_delta.length() > 0.01f && localpawn->m_iShotsFired() > 1;

            QAngle smoothed_angle;
            if (weapon_config.smooth_mode == 0)
                smoothed_angle = viewangles.linear_smooth(target.target_angle, weapon_config.smooth);
            else if (weapon_config.smooth_mode == 1)
                smoothed_angle = viewangles.const_smooth(target.target_angle, weapon_config.smooth);
            else if (weapon_config.smooth_mode == 2)
                smoothed_angle = viewangles.lerp(target.target_angle, t);
                
            smoothed_angle.normalize_clamp();
                
            g::client->SetViewAngles(smoothed_angle);
            
            if (t >= 1.0f)
                timer.reset();
        }
        rcs(localpawn, viewangles, cmd, in_fov);
    }
}