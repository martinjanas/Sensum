#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/interfaces/CNetworkGameClient.h"

class CNetworkGameService : public Interface
{
public:
	CNetworkGameService(void* obj, const char* name) : Interface(obj, name) {}
	
	CNetworkGameClient* GetNetworkGameClient()
	{
		//return *reinterpret_cast<CNetworkGameClient**>(reinterpret_cast<uintptr_t>(this->m_obj) + 0xB8);

		return VTable::GetThiscall<CNetworkGameClient*>(this->m_obj, 23);
	}
};
