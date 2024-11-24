#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/interfaces/CNetworkGameClient.h"

class CNetworkGameService
{
public:
	CNetworkGameClient* GetNetworkGameClient()
	{
		//return *reinterpret_cast<CNetworkGameClient**>(reinterpret_cast<uintptr_t>(this) + 0xB8);

		return VTable::GetThiscall<CNetworkGameClient*>(this, 23);
	}
};
