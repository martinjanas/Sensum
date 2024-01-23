#pragma once

class IBaseClientDLL
{
public:
	ClientClass* GetAllClasses()
	{
		return CallVFunction<ClientClass * (__thiscall*)(void*)>(this, 8)(this);
	}
};
