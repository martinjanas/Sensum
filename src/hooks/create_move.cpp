#include "hooks.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/entities.h"
#include "../features/features.h"
#include "../helpers/runtime_saver.h"
#pragma intrinsic(_ReturnAddress)

float side = 1.0f;

static float next_lby = 0.0f;

float real_angle = 0.0f;
float view_angle = 0.0f;

static CCSGOPlayerAnimState g_AnimState;
static int max_choke_ticks = 8; //was 6

char buf_[256];
bool done = false;
int count = 0;

float AngleDiff(float destAngle, float srcAngle) {
	float delta;

	delta = fmodf(destAngle - srcAngle, 360.0f);
	if (destAngle > srcAngle) {
		if (delta >= 180)
			delta -= 360;
	}
	else {
		if (delta <= -180)
			delta += 360;
	}
	return delta;
}

namespace hooks
{
	void __stdcall create_move::hooked(int sequence_number, float input_sample_frametime, bool active, bool sendpacket)
	{
		original(g::base_client, sequence_number, input_sample_frametime, active);

		CUserCmd* cmd = g::input->GetUserCmd(sequence_number);
		CVerifiedUserCmd* verified = g::input->GetVerifiedCmd(sequence_number);

		if (!cmd || !cmd->command_number)
			return;

		if (settings::misc::bhop)
			features::bhop(cmd);

		if (settings::misc::auto_strafe)
			features::auto_strafe(cmd);

		if (settings::misc::smoke_helper)
			visuals::SmokeHelperAimbot(cmd);

		if (settings::misc::flash_helper)
			visuals::PopflashHelperAimbot(cmd);

		slow_walk::handle(cmd);

		entities::on_create_move(cmd);
		features::edge_jump_pre(cmd);
		engine_prediction::start(cmd);

		visuals::fetch_entities();
		entities::fetch_targets(cmd);

		static int latency_ticks = 0;
		float fl_latency = g::engine_client->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);
		int latency = TIME_TO_TICKS(fl_latency);
		if (g::client_state->chokedcommands <= 0) {
			latency_ticks = latency;
		}
		else {
			latency_ticks = std::max(latency, latency_ticks);
		}

		if (g::game_rules_proxy->m_bIsValveDS()) {
			if (fl_latency >= g::global_vars->interval_per_tick)
				max_choke_ticks = 11 - latency_ticks;
			else
				max_choke_ticks = 11;
		}
		else {
			max_choke_ticks = 13 - latency_ticks; //was 5
		}

		static float SpawnTime = 0.0f;
		if (g::local_player->m_flSpawnTime() != SpawnTime) {
			g_AnimState.pBaseEntity = g::local_player;
			g::local_player->ResetAnimationState(&g_AnimState);
			SpawnTime = g::local_player->m_flSpawnTime();
		}

		QAngle OldAngles = cmd->viewangles;

		auto Desync = [OldAngles, &sendpacket](CUserCmd* cmd)
		{
			if (cmd->buttons & (IN_ATTACK | IN_ATTACK2 | IN_USE) ||
				g::local_player->m_nMoveType() == MOVETYPE_LADDER || g::local_player->m_nMoveType() == MOVETYPE_NOCLIP
				|| !g::local_player->IsAlive())
				return;

			if (g::local_player->m_bGunGameImmunity() || g::local_player->m_fFlags() & FL_FROZEN)
				return;

			if (!utils::IsPlayingMM_AND_IsValveServer())
				return;

			auto weapon = g::local_player->m_hActiveWeapon().Get();
			if (!weapon)
				return;

			auto weapon_index = weapon->m_iItemDefinitionIndex();
			if ((weapon_index == WEAPON_GLOCK || weapon_index == WEAPON_FAMAS || weapon_index == WEAPON_REVOLVER) && weapon->m_flNextPrimaryAttack() >= g::global_vars->curtime)
				return;

			auto weapon_data = weapon->get_weapon_data();

			if (weapon_data->WeaponType == WEAPONTYPE_GRENADE) {
				if (!weapon->m_bPinPulled()) {
					float throwTime = weapon->m_fThrowTime();
					if (throwTime > 0.f)
						return;
				}

				if ((cmd->buttons & IN_ATTACK) || (cmd->buttons & IN_ATTACK2)) {
					if (weapon->m_fThrowTime() > 0.f)
						return;
				}
			}

			static bool broke_lby = false;

			if (next_lby >= g::global_vars->curtime) {
				if (!broke_lby && sendpacket && g::client_state->chokedcommands > 0)
					return;

				broke_lby = false;
				sendpacket = false;
				cmd->viewangles.yaw += 120.0f * side; //was 120.f and side
			}
			else {
				broke_lby = true;
				sendpacket = false;
				cmd->viewangles.yaw += 120.0f * -side; //was 120.f and -side
			}
			math::FixAngles(cmd->viewangles);
			math::MovementFix(cmd, OldAngles, cmd->viewangles);
		};

		if (settings::visuals::grenade_prediction)
			grenade_prediction::fetch_points(cmd);

		if (settings::misc::fast_stop)
			features::fast_stop(cmd);

		if (cmd->weaponselect == 0)
		{
			aimbot::handle(cmd);
			zeusbot::handle(cmd);
		}

		static int definition_index = 7;
		auto a_settings = &settings::aimbot::m_items[definition_index];

		if (settings::chams::enemy::backtrack_chams || settings::chams::teammates::backtrack_chams)
			aimbot::get_backtrack_data(cmd);

		if (settings::misc::smoke_helper)
			visuals::SmokeHelperAimbot(cmd);

		if (settings::misc::flash_helper)
			visuals::PopflashHelperAimbot(cmd);

		if (g::local_player && g::local_player->IsAlive() && (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2))
			saver.LastShotEyePos = g::local_player->GetEyePos();

		if (settings::misc::lefthandknife)
			visuals::KnifeLeft();

		if (settings::desync::enabled && std::fabsf(g::local_player->m_flSpawnTime() - g::global_vars->curtime) > 1.0f)
			Desync(cmd);

		math::FixAngles(cmd->viewangles);
		cmd->viewangles.yaw = std::remainderf(cmd->viewangles.yaw, 360.0f);

		if (settings::desync::enabled && g::client_state->chokedcommands >= max_choke_ticks) {
			sendpacket = true;
			cmd->viewangles = g::client_state->viewangles;
		}

		auto anim_state = g::local_player->GetPlayerAnimState();
		if (anim_state) {
			CCSGOPlayerAnimState anim_state_backup = *anim_state;
			*anim_state = g_AnimState;
			*g::local_player->GetVAngles2() = cmd->viewangles;
			g::local_player->UpdateClientSideAnimation();

			if (anim_state->speed_2d > 0.1f || std::fabsf(anim_state->flUpVelocity)) {
				next_lby = g::global_vars->curtime + 0.22f;
			}
			else if (g::global_vars->curtime > next_lby) {
				if (std::fabsf(AngleDiff(anim_state->m_flGoalFeetYaw, anim_state->m_flEyeYaw)) > 35.0f) {
					next_lby = g::global_vars->curtime + 1.1f;
				}
			}

			g_AnimState = *anim_state;
			*anim_state = anim_state_backup;
		}

		if (sendpacket) {
			real_angle = g_AnimState.m_flGoalFeetYaw;
			view_angle = g_AnimState.m_flEyeYaw;
		}

		engine_prediction::finish(cmd);
		features::edge_jump_post(cmd);

		cmd->viewangles.pitch = std::clamp(cmd->viewangles.pitch, -89.0f, 89.0f);
		cmd->viewangles.yaw = std::clamp(cmd->viewangles.yaw, -180.0f, 180.0f);
		cmd->viewangles.roll = 0.0f;
		cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
		cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
		cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

		globals::viewangles = cmd->viewangles;

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}
}