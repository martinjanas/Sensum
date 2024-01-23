#pragma once

class IMaterialVar
{
public:
	void SetFloatValue(float value)
	{
		CallVFunction<void(__thiscall*)(void*, float)>(this, 4)(this, value);
	}

	void SetIntValue(int value)
	{
		CallVFunction<void(__thiscall*)(void*, int)>(this, 5)(this, value);
	}

	void SetStringValue(char const* value)
	{
		CallVFunction<void(__thiscall*)(void*, char const*)>(this, 6)(this, value);
	}

	void SetVectorValue(float x, float y, float z)
	{
		CallVFunction<void(__thiscall*)(void*, float, float, float)>(this, 11)(this, x, y, z);
	}

	void SetVecComponentValue(float value, int component)
	{
		CallVFunction<void(__thiscall*)(void*, float, int)>(this, 26)(this, value, component);
	}
};
