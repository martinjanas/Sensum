#pragma once

#include "../math/Vector.h"
#include "../helpers/vfunc.h"
#include "../math/QAngle.h"
#include "../interfaces/CGameTrace.h"

class CSource2Client
{
public:
	void GetViewAngles(QAngle* vec)
	{
		GetVirtual<void(__thiscall*)(void*, int, QAngle*)>(this, 77)(this, 0, vec);
	}

	void SetViewAngles(QAngle& vec)
	{
		vec.normalize_clamp();

		GetVirtual<void(__thiscall*)(void*, int, QAngle&)>(this, 78)(this, 0, vec);
	}

	//bool TraceRay(const Vector& vecAbsStart, const Vector& vecAbsEnd, CTraceFilter* filter, Trace_t* trace)
	//{
	//	//   @Note: this method initializes the trace filter vtable itself but copies the rest from argument //100
	//	return GetVirtual<bool(__thiscall*)(void*, const Vector&, const Vector&, CTraceFilter*, Trace_t*)>(this, 108)(this, vecAbsEnd, vecAbsEnd, &filter[1], trace);
	//}
};