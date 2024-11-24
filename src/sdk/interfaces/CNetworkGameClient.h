#pragma once
#include "../../sdk/helpers/vfunc.h"
#include "../../sdk/interfaces/CGlobalVars.h"

class CNetworkGameClient
{
public:
	/*CGlobalVarsBase* GetGlobalVars()
	{
		//xref: Curtime( %f )\nRealtime( %f )\n module: engine2
		return VTable::GetThiscall<CGlobalVarsBase*>(this, 4);
	}*/

	void FullUpdate()
	{
		//   @ida: #STR: "Requesting full game update (%s)...\n"
		VTable::GetThiscall<void>(this, 28, "unk");
	}
};