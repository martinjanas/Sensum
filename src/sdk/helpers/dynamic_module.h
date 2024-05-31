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

using InstantiateInterface = void*(__cdecl*)();

struct InterfaceReg
{
	InstantiateInterface m_pCreateFn;
	const char* m_pName;
	InterfaceReg* m_pNext;	
};


class Exporter
{
public:
	Exporter(HMODULE _base) : base(_base) { }
	Exporter() { }

	Exporter& get_export(const std::string_view& func_name)
	{
		this->addr = reinterpret_cast<uint8_t*>(LI_FN(GetProcAddress).cached()(this->base, func_name.data()));

		return *this;
	}

	template <typename T = void*>
	T* as()
	{
		if (!addr)
			return nullptr;

		return reinterpret_cast<T*>(addr);
	}

private:
	HMODULE base;
	uint8_t* addr;
};

class PatternScanner
{
public:
	PatternScanner(HMODULE base)
	{
		this->base = base;
	}

	PatternScanner() { }

	PatternScanner& scan(const char* signature)
	{
		this->addr = pattern_scan(signature);

		return *this;
	}

	PatternScanner& scan(const char* signature, const char* sig_name)
	{
		this->addr = pattern_scan(signature);

		if (!this->addr)
		{
			printf_s("PatternScanner: %s not found\n", sig_name);
		}

		return *this;
	}

	PatternScanner& abs()
	{
		const int relative = *reinterpret_cast<int*>(this->addr);

		this->addr += relative + sizeof(relative);

		return *this;
	}

	PatternScanner& add(const uintptr_t& offset)
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
	std::uint8_t* pattern_scan(const char* signature) const
	{
		static auto pattern_to_byte = [](const char* pattern)
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

		auto offset_ptr = [](auto* base, std::size_t offset) -> PIMAGE_NT_HEADERS
			{
				return reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(base) + offset);
			};

		auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(this->base);
		auto nt_headers = offset_ptr(dos_header, dos_header->e_lfanew);

		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte(signature);
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

	const char* pattern;
	HMODULE base;
	uint8_t* addr;
	//std::optional<uint8_t*> addr;
};

static HMODULE GetModuleBase(const char* module_name)
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
			return (HMODULE)entry->DllBase;
	}

	return nullptr;
}

class DynamicModule
{
public:
	DynamicModule(const std::string_view& dll_name)
	{
		this->base = GetModuleBase(dll_name.data());
		this->base_addr = reinterpret_cast<uintptr_t>(this->base);
		this->dll_name = dll_name.data();

		pattern_scanner = PatternScanner(this->base);
		exporter = Exporter(this->base);
	}

	DynamicModule() { }

	template<typename T>
	T GetInterface(const std::string_view& interface_name)
	{
		using fn = void* (*)(const char* pName, int* pReturnCode);

		auto CreateInterface = exporter.get_export("CreateInterface").as<fn>();

		if (!CreateInterface)
			return nullptr;

		return reinterpret_cast<T>(CreateInterface(interface_name.data(), 0));
	}

	template<typename T>
	T GetInterfaceFromList(const std::string_view& interface_name)
	{
		auto CreateInterface = exporter.get_export("CreateInterface").as<uint8_t>();

		if (!CreateInterface)
			return nullptr;

		InterfaceReg* reg = *reinterpret_cast<InterfaceReg**>(resolve_rip(CreateInterface, std::nullopt, std::nullopt));

		if (!reg)
			return nullptr;

		for (auto& it = reg; it != nullptr; it = it->m_pNext)
		{
			if (interface_name.find(it->m_pName) != std::string_view::npos)
				return reinterpret_cast<T>(it->m_pCreateFn());
		}

		return nullptr;
	}

	void PrintAllInterfaces()
	{
		auto CreateInterface = exporter.get_export("CreateInterface").as<uint8_t>();
		if (!CreateInterface)
			return;
		
		InterfaceReg* reg = *reinterpret_cast<InterfaceReg**>(resolve_rip(CreateInterface, std::nullopt, std::nullopt));
		if (!reg)
			return;

		for (auto& it = reg; it != nullptr; it = it->m_pNext)
		{
			printf("name: %s\n", it->m_pName);
		}
	}

	static uint8_t* resolve_rip(uint8_t* address, std::optional<size_t> offset = std::nullopt, std::optional<size_t> length = std::nullopt)
	{
		uint32_t displacement = *reinterpret_cast<uint32_t*>(address + (offset.value_or(0x3)));

		return address + (length.value_or(0x7)) + static_cast<size_t>(displacement);
	}

	HMODULE base;
	uintptr_t base_addr;
	const char* dll_name;
	PatternScanner pattern_scanner;
	Exporter exporter;
};





