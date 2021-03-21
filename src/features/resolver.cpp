#include "features.h"
#include "../helpers/console.h"

namespace resolver
{
	void handle()
	{
		if (!settings::desync::resolver)
			return;

		for (int i = 1; i < g::engine_client->GetMaxClients(); ++i)
		{
			auto player = c_base_player::GetPlayerByIndex(i);
			if (player && player->IsPlayer() && player != g::local_player)
			{
				auto state = player->GetPlayerAnimState();
				//player->m_angEyeAngles().yaw = player->m_flLowerBodyYawTarget();
				player->m_angEyeAngles().yaw = state->m_flGoalFeetYaw;
				player->m_angEyeAngles().NormalizeClamp();
			}
		}
	}
}