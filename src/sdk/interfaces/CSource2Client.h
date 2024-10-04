#pragma once

#include "../math/Vector.h"
#include "../helpers/vfunc.h"
#include "../math/QAngle.h"
#include "../interfaces/CGameTrace.h"

class CSource2Client
{
public:
	void GetViewAngles(int slot, QAngle* vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, QAngle*)>(this, 77)(this, slot, vec);
	}

	void SetViewAngles(int slot, const QAngle& vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, const QAngle&)>(this, 78)(this, slot, vec);
	}

	bool TraceRay(const Vector& vecAbsStart, const Vector& vecAbsEnd, TraceFilter_t* filter, Trace_t* trace)
	{
		//   @Note: this method initializes the trace filter vtable itself but copies the rest from argument //100
		return GetVirtual<bool(__thiscall*)(void*, const Vector&, const Vector&, TraceFilter_t*, Trace_t*)>(this, 108)(this, vecAbsEnd, vecAbsEnd, &filter[1], trace);
	}
};