#pragma once
#include <string_view>
#include <wtypes.h>
#include "../../sdk/helpers/importer.h"

class DExporter
{
public:
	DExporter() = default;
	DExporter(uintptr_t _base) : m_Base(_base) {}

	DExporter& Get(const std::string_view& func_name)
	{
		this->m_Addr = reinterpret_cast<uint8_t*>(LI_FN(GetProcAddress).cached()((HMODULE)this->m_Base, func_name.data()));

		return *this;
	}

	DExporter& Abs()
	{
		const int relative = *reinterpret_cast<int*>(this->m_Addr);

		this->m_Addr += relative + sizeof(relative);

		return *this;
	}

	DExporter& Add(const uintptr_t& offset)
	{
		if (this->m_Addr)
			this->m_Addr += offset;

		return *this;
	}

	template <typename T = void*>
	T As()
	{
		if (!m_Addr)
			return {};

		return reinterpret_cast<T>(m_Addr);
	}

private:
	uintptr_t m_Base;
	uint8_t* m_Addr;
};


