#pragma once

#include "GameEntitySystem.h"

class CGameResourceService : public Interface
{
public:
	CGameResourceService(void* obj, const char* name) : Interface(obj, name) { }
	
	CEntitySystem* GetEntitySystem()
	{
		return *reinterpret_cast<CEntitySystem**>(reinterpret_cast<uintptr_t>(this->m_obj) + 0x58);
	}
};
