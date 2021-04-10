#include "hooks.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/entities.h"
#include "../features/features.h"
#include "../helpers/runtime_saver.h"
#pragma intrinsic(_ReturnAddress)

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

		slow_walk::handle(cmd);

		if (settings::misc::smoke_helper)
			visuals::SmokeHelperAimbot(cmd);

		if (settings::misc::flash_helper)
			visuals::PopflashHelperAimbot(cmd);

		features::edge_jump_pre(cmd);
		engine_prediction::start(cmd);

		entities::on_create_move(cmd);
		
		visuals::fetch_entities();
		entities::fetch_targets(cmd);

		if (settings::visuals::grenade_prediction)
			grenade_prediction::fetch_points(cmd);

		if (settings::misc::fast_stop)
			features::fast_stop(cmd);

		features::selfnade(cmd);

		if (cmd->weaponselect == 0)
		{
			aimbot::handle(cmd);
			zeusbot::handle(cmd);
		}

		if (settings::chams::enemy::backtrack_chams || settings::chams::teammates::backtrack_chams)
			aimbot::get_backtrack_data(cmd);

		if (settings::misc::smoke_helper)
			visuals::SmokeHelperAimbot(cmd);

		if (settings::misc::flash_helper)
			visuals::PopflashHelperAimbot(cmd);

		if (g::local_player && g::local_player->IsAlive() && (cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2))
			saver.LastShotEyePos = g::local_player->GetEyePos();

		if (settings::misc::lefthandknife)
			visuals::leftknife();

		engine_prediction::finish(cmd);
		features::edge_jump_post(cmd);

		cmd->viewangles.pitch = std::clamp(cmd->viewangles.pitch, -89.0f, 89.0f);
		cmd->viewangles.yaw = std::clamp(cmd->viewangles.yaw, -180.0f, 180.0f);
		cmd->viewangles.roll = 0.0f;
		cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
		cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
		cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

		cmd->viewangles.Normalize();

		globals::viewangles = cmd->viewangles;

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}
}