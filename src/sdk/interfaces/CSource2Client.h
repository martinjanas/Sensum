#pragma once

#include "../math/Vector.h"
#include "../helpers/vfunc.h"
#include "../math/QAngle.h"

class CSource2Client
{
public:
	void GetViewAngles(int slot, QAngle* vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, QAngle*)>(this, 74)(this, slot, vec);
	}

	void SetViewAngles(int slot, const QAngle& vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, const QAngle&)>(this, 75)(this, slot, vec);
	}
};