#include "Module.h"
#include <Windows.h>
#include <cstdint>
#include <winternl.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

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

Module::Module(std::string_view mod_name)
{
	m_Base = GetModuleBase(mod_name.data());
	m_Exporter = DExporter(m_Base);
}
