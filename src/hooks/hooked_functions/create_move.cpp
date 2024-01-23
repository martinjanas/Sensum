#include "../hooks.h"
#include "../../settings/globals.h"
#include "../../settings/options.hpp"
#include "../../helpers/input.h"
#include "../../helpers/console.h"
#include "../../helpers/entities.h"
#include "../../features/features.h"
#include "../../valve_sdk/classids.h"

#include <algorithm>

namespace hooks
{
	bool __fastcall create_move::hooked(void* ecx, void* edx, float input_sample_frametime, CUserCmd* cmd)
	{
		bool ret = original(ecx, input_sample_frametime, cmd);

		if (!cmd || !cmd->command_number || !g::local_player)
			return ret;

		if (ret)
		{
			cmd->viewangles.NormalizeClamp();

			g::engine_client->SetViewAngles(cmd->viewangles);
			g::prediction->SetLocalViewAngles(cmd->viewangles);
		}

		entities::fetch_targets(cmd);

		if (settings::visuals::grenade_prediction)
			grenade_prediction::fetch_points(cmd);

		if (settings::misc::fast_stop)
			features::fast_stop(cmd);

		if (settings::misc::bhop)
			features::bhop(cmd);

		if (settings::misc::auto_strafe)
			features::auto_strafe(cmd);

		if (settings::misc::selfnade)
			features::selfnade(cmd);

		if (cmd->weaponselect == 0)
		{
			aimbot::handle(cmd);

			if (settings::misc::smoke_helper)
				visuals::SmokeHelperAimbot(cmd);

			if (settings::misc::flash_helper)
				visuals::PopflashHelperAimbot(cmd);
		}

		cmd->viewangles.NormalizeClamp();

		cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
		cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
		cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

		return false;
	}
}