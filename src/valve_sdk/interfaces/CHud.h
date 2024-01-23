#pragma once

class CHud
{
public:
	template <typename T>
	T* FindHudElement(const char* name)
	{
		static auto fn = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(patterns::find_hud_addr);

		return (T*)fn(this, name);
	}
};
