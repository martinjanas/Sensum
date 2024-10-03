#pragma once

#include "GameEntitySystem.h"

class CGameResourceService
{
public:
	CEntitySystem* GetEntitySystem()
	{
		return *reinterpret_cast<CEntitySystem**>(reinterpret_cast<uintptr_t>(this) + 0x58);
	}
};
