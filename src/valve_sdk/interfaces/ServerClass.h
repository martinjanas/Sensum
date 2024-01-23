#pragma once

#include "../../valve_sdk/misc/Enums.hpp"
#include "../../valve_sdk/misc/vfunc.hpp"

struct ServerClass
{
	char* m_pNetworkName;
	void* m_pTable; //SendTable*
	ServerClass* m_pNext;
	EClassId m_ClassID;
	int m_InstanceBaselineIndex;
};

struct ServerClassDLL
{
	ServerClass* GetAllClasses()
	{
		return CallVFunction<ServerClass* (__thiscall*)(void*)>(this, 10)(this);
	}
};
