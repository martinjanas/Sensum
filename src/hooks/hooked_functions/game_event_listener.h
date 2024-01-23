#pragma once

#include "../../valve_sdk/csgostructs.hpp"
#include "../../settings/globals.h"
#include "../../features/features.h"

class CGameEventListener final : public IGameEventListener2
{
	void FireGameEvent(IGameEvent* context) override;

	int GetEventDebugID(void) override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};
