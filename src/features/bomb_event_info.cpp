#include "features.h"
#include "../helpers/notifies.h"
#include "../settings/globals.h"

#include <format>

namespace features
{
	void on_bomb_begindefuse(IGameEvent* event)
	{
		auto entity = c_base_player::GetPlayerByUserId(event->GetInt("userid"));
		if (!entity || !g::local_player || entity == g::local_player)
			return;

		float dist_to_a = entity->GetAbsOrigin().DistTo((*g::player_resource)->m_bombsiteCenterA());
		float dist_to_b = entity->GetAbsOrigin().DistTo((*g::player_resource)->m_bombsiteCenterB());

		static std::string site;

		if (dist_to_a < dist_to_b)
			site = "A";

		if (dist_to_b < dist_to_a)
			site = "B";

		notifies::push(std::format("{:.5s} is defusing on {:s} {:s}", entity->GetPlayerInfo().szName, site, event->GetBool("haskit") ? "(kit)" : ""));
	}
}