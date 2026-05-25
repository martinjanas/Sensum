#pragma once
#include <string_view>
#include "../helpers/Exporter.h"

using InstantiateInterfaceFn2 = void* (__cdecl*)();

struct InterfaceReg2
{
	InstantiateInterfaceFn2 m_pCreateFn;
	const char* m_pName;
	InterfaceReg2* m_pNext;
};

class Module
{
public:
	Module() = default;
	Module(std::string_view mod_name);

public:

	template<typename T>
	T GetInterface(const std::string_view& interface_name)
	{
		auto interface_registry = *m_Exporter.Get("CreateInterface").Add(0x3).Abs().As<InterfaceReg2**>();
		if (!interface_registry)
			return nullptr;

		for (auto& reg = interface_registry; reg != nullptr; reg = reg->m_pNext)
		{
			if (interface_name.find(reg->m_pName) != std::string_view::npos)
				return reinterpret_cast<T>(reg->m_pCreateFn());
		}

		return nullptr;
	}

	uintptr_t& GetBase() { return m_Base; }
	DExporter& GetExport(const std::string_view& func_name) { return m_Exporter.Get(func_name); }

private:
	uintptr_t m_Base;
	DExporter m_Exporter;
};
