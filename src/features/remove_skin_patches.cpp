#include "features.h"
#include "../helpers/entities.h"

namespace features
{
	entities::player_data_t m_entities[MAX_PLAYERS];

	void remove_skin_patches()
	{
		if (!settings::chams::enemy::enabled)
			return;

		if (!g::local_player)
			return;

		if (entities::locker.try_lock())
		{
			memcpy(m_entities, entities::m_items.front().players, sizeof(m_entities));
			entities::locker.unlock();
		}

		for (auto& data : m_entities)
		{
			if (!data.player)
				continue;

			if (!data.is_player)
				continue;

			if (!data.is_alive)
				continue;

			for (int i = 0; i < MAX_PATCHES; i++)
			{
				data.player->m_vecPlayerPatchEconIndices()[i] = NULL;
			}
		}
	}
}