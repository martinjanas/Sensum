#pragma once
#include <string>
#include <optional>
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <map>
#include <vector>
#include <stdexcept>
#include <format>
#include <winternl.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include "../helpers/importer.h"

#pragma warning(disable:26495)

using InstantiateInterfaceFn = void*(__cdecl*)();

struct InterfaceReg
{
	InstantiateInterfaceFn m_pCreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;	
};

static uintptr_t GetModuleBase(const char* module_name)
{
	PPEB peb = (PPEB)__readgsqword(0x60);
	PPEB_LDR_DATA ldr = peb->Ldr;
	PLIST_ENTRY list = &ldr->InMemoryOrderModuleList;

	for (PLIST_ENTRY item = list->Flink; item != list; item = item->Flink)
	{
		PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(item, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		char module_buffer[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, entry->FullDllName.Buffer, entry->FullDllName.Length / sizeof(WCHAR), module_buffer, MAX_PATH, NULL, NULL);
		module_buffer[entry->FullDllName.Length / sizeof(WCHAR)] = '\0';

		const auto& module_filename = PathFindFileNameA(module_buffer);
		if (!strcmp(module_filename, module_name))
			return (uintptr_t)entry->DllBase;
	}

	return 0;
}

class Exporter
{
public:
	Exporter() = default;
	Exporter(uintptr_t _base) : base(_base) { }

	Exporter& get_export(const std::string_view& func_name)
	{
		this->addr = reinterpret_cast<uint8_t*>(LI_FN(GetProcAddress).cached()((HMODULE)this->base, func_name.data()));

		return *this;
	}

	Exporter& abs()
	{
		const int relative = *reinterpret_cast<int*>(this->addr);

		this->addr += relative + sizeof(relative);

		return *this;
	}

	Exporter& add(const uintptr_t& offset)
	{
		if (this->addr)
			this->addr += offset;

		return *this;
	}

	template <typename T = void*>
	T as()
	{
		if (!addr)
			return {};

		return reinterpret_cast<T>(addr);
	}

private:
	uintptr_t base;
	uint8_t* addr;
};

class PatternScanner
{
public:
	PatternScanner() = default;
	PatternScanner(uintptr_t _base) : base(_base) { }

	PatternScanner& scan(const char* signature, const char* sig_name)
	{
		this->addr = pattern_scan(signature);

		if (!this->addr)
		{
			printf_s("PatternScanner: %s not found\n", sig_name);
		}

		return *this;
	}

	PatternScanner& add(const uintptr_t& offset)
	{
		if (this->addr)
			this->addr += offset;

		return *this;
	}

	PatternScanner& abs()
	{
		const int relative = *reinterpret_cast<int*>(this->addr);

		this->addr += relative + sizeof(relative);

		return *this;
	}

	template <typename T = void*>
	T as()
	{
		if (!addr)
			return {};

		return reinterpret_cast<T>(addr);
	}
private:
	std::uint8_t* pattern_scan(const char* signature) const
	{
		static auto pattern_to_bytes = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;

					if (*current == '?')
						++current;

					bytes.emplace_back(-1);
				}
				else bytes.emplace_back(strtoul(current, &current, 16));
			}
			return bytes;
		};

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(this->base);
		auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(dos_header) + dos_header->e_lfanew);

		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_bytes(signature);
		auto scan_bytes = reinterpret_cast<uint8_t*>(this->base);

		for (std::size_t i = 0; i < size_of_image - pattern_bytes.size(); ++i)
		{
			bool found = true;
			for (std::size_t j = 0; j < pattern_bytes.size(); ++j)
			{
				if (scan_bytes[i + j] != pattern_bytes[j] && pattern_bytes[j] != -1)
				{
					found = false;
					break;
				}
			}

			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	uintptr_t base;
	uint8_t* addr;
};

class DynamicModule
{
public:
	DynamicModule() = default;
	DynamicModule(const std::string_view& mod_name) : base(GetModuleBase(mod_name.data())), pattern_scanner(base), exporter(base) { }
	
	template<typename T>
	T GetInterfaceFromList(const std::string_view& interface_name)
	{
		auto interface_registry = *exporter.get_export("CreateInterface").add(0x3).abs().as<InterfaceReg**>();
		if (!interface_registry)
			return nullptr;

		for (auto& reg = interface_registry; reg != nullptr; reg = reg->m_pNext)
		{
			if (interface_name.find(reg->m_pName) != std::string_view::npos)
				return reinterpret_cast<T>(reg->m_pCreateFn());
		}

		return nullptr;
	}

	void PrintAllInterfaces();
	Exporter& get_export(const std::string_view& func_name);
	PatternScanner& scan(const char* signature, const char* msg);

	uintptr_t base;
private:
	PatternScanner pattern_scanner;
	Exporter exporter;
};





