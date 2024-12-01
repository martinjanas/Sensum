#pragma once
#include <cstddef>
#include "../sdk.h"

class CUtlBuffer
{
public:
	std::byte pad01[0x80];

	CUtlBuffer(int a1, int size, int a3)
	{
		using fn = void(__thiscall*)(void*, int, int, int);
		static const auto& addr = modules::tier0.get_export("CUtlBuffer::CUtlBuffer").as();

		const auto constructor = reinterpret_cast<fn>(addr);
		if (constructor)
			constructor(this, a1, size, a3);
	}

	void PutString(const char* string)
	{
		using fn = void(__thiscall*)(void*, const char*);
		static const auto& addr = modules::tier0.get_export("CUtlBuffer::PutString").as();

		const auto put_string = reinterpret_cast<fn>(addr);
		if (put_string)
			put_string(this, string);
	}

	void EnsureCapacity(int size)
	{
		using fn = void(__thiscall*)(void*, int);
		static const auto& addr = modules::tier0.get_export("CUtlBuffer::EnsureCapacity").as();

		const auto ensure_capacity = reinterpret_cast<fn>(addr);
		if (ensure_capacity)
			ensure_capacity(this, size);
	}
};