#pragma once

class IPanel
{
public:
	const char* GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall * tGetName)(void*, unsigned int);
		return CallVFunction<tGetName>(this, 36)(this, vguiPanel);
	}

	const char* GetClassName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall * tGetClassName)(void*, unsigned int);
		return CallVFunction<tGetClassName>(this, 37)(this, vguiPanel);
	}
};