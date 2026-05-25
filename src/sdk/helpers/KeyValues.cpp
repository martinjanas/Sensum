#include "KeyValues.h"
#include "../../core/helpers/Modules2.h"

bool KeyValues::LoadKV3(KeyValues* kv, const char* material_vmat, const char* kv_name)
{
	using fn = bool(__fastcall*)(void* thisptr, void* utlstring, const char* buffer, const KV3ID_t* format, const char* kv_name);
	const auto addr = modules2::tier02.GetExport("?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z").As();
	if (!addr)
		return false;

	auto load_kv3 = reinterpret_cast<fn>(addr);
	return load_kv3(kv, nullptr, material_vmat, &g_KV3Format_Generic, kv_name);
}