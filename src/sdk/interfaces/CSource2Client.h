#pragma once

#include "../math/Vector.h"
#include "../helpers/vfunc.h"
#include "../math/QAngle.h"

class CSource2Client
{
public:
	void GetViewAngles(int slot, QAngle* vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, QAngle*)>(this, 77)(this, slot, vec);
	}

	void SetViewAngles(int slot, QAngle& vec)
	{
		vec.clamp_normalize();
		GetVirtual<void(__thiscall*)(void*, int, const QAngle&)>(this, 78)(this, slot, vec);
	}
};