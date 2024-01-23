#pragma once

#include <iostream>

#include "../../valve_sdk/misc/vfunc.hpp"

class IInputSystem
{
public:
	void EnableInput(bool enable)
	{
		return CallVFunction<void(__thiscall*)(void*, bool)>(this, 11)(this, enable);
	}

	void*& GetWindow()
	{
		static uint32_t offset = 0;
		if (!offset)
			offset = *reinterpret_cast<uint32_t*>((*reinterpret_cast<char***>(this))[10] + 5);

		return *reinterpret_cast<void**>(reinterpret_cast<char*>(this) + offset);
	}
};
