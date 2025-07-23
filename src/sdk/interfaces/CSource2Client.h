#pragma once

#include "../math/Vector.h"
#include "../helpers/vfunc.h"
#include "../math/QAngle.h"
#include "../interfaces/CGameTrace.h"

class CSource2Client : public Interface
{
public:
	CSource2Client(void* obj, const char* name) : Interface(obj, name) { }
	void GetViewAngles(QAngle* vec)
	{
		VTable::GetThiscall<void>(this->m_obj, 77, 0, vec);
	}

	void SetViewAngles(QAngle& vec)
	{
		vec.normalize_clamp();
		VTable::GetThiscall<void>(this->m_obj, 78, 0, vec);
	}

	//bool TraceRay(const Vector& vecAbsStart, const Vector& vecAbsEnd, CTraceFilter* filter, Trace_t* trace)
	//{
	//	//   @Note: this method initializes the trace filter vtable itself but copies the rest from argument //100
	//	return GetVirtual<bool(__thiscall*)(void*, const Vector&, const Vector&, CTraceFilter*, Trace_t*)>(this->m_obj, 108)(this, vecAbsEnd, vecAbsEnd, &filter[1], trace);
	//}
};