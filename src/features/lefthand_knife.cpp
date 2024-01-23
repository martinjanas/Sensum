#include "features.h"

namespace features
{
	void lefthand_knife()
	{
		static auto left_knife = g::cvar->find("cl_righthand");

		if (!g::local_player || !g::local_player->IsAlive())
		{
			left_knife->SetValue(1);
			return;
		}

		const auto& weapon = g::local_player->m_hActiveWeapon();

		if (!weapon)
			return;

		left_knife->SetValue(!weapon->IsKnife());
	}
}