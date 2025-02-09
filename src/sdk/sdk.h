#pragma once
#include <dxgi.h>
#include <cstddef>
#include "../sdk/interfaces/IVEngineClient.h"
#include "../sdk/interfaces/SchemaSystem.h"
#include "../sdk/interfaces/IMemAlloc.h"
#include "../sdk/interfaces/CGameResourceService.h"
#include "../sdk/interfaces/CSGOInput.h"
#include "../sdk/interfaces/CSource2Client.h"
#include "../sdk/interfaces/CInputSystem.h"
#include "../sdk/interfaces/CGameTrace.h"
#include "../sdk/interfaces/GameEntitySystem.h"
#include "../sdk/interfaces/CGameRules.h"
#include "../sdk/interfaces/CRenderSystem.h"
#include "../sdk/interfaces/ClientModeCSNormal.h"
#include "../sdk/interfaces/CNetworkGameService.h"
#include "../sdk/interfaces/CGameType.h"
#include "../sdk/interfaces/CHudChat.h"
#include "../sdk/interfaces/IVEngineCVar.h"

namespace sdk
{
	void init_modules();
	void init_interfaces();
	void validate_interfaces_on_map_load();
	void invalidate_interfaces_on_map_unload();
	void scan_and_cache_sigs();
}

struct KV3ID_t
{
	const char* encoding_type; //0x0
	uint64_t key1; //0x8
	uint64_t key2; //0x10

	KV3ID_t(const char* encoding_type, uint64_t key1, uint64_t key2)
	{
		this->encoding_type = encoding_type;
		this->key1 = key1;
		this->key2 = key2;
	}
};

// encodings
const KV3ID_t g_KV3Encoding_Text = { "text", 0x41C58A33E21C7F3C, 0xDAA323A6DA77799 };
const KV3ID_t g_KV3Encoding_Binary = { "binary", 0x40C1F7D81B860500, 0x14E76782A47582AD };
const KV3ID_t g_KV3Encoding_BinaryLZ4 = { "binary_lz4", 0x4F5C63A16847348A, 0x19B1D96F805397A1 };
const KV3ID_t g_KV3Encoding_BinaryZSTD = { "binary_zstd", 0x4305FEF06F620A00, 0x29DBB14623045FA3 };
const KV3ID_t g_KV3Encoding_BinaryBC = { "binary_bc", 0x4F6C95BC95791A46, 0xD2DFB7A1BC050BA7 };
const KV3ID_t g_KV3Encoding_BinaryAuto = { "binary_auto", 0x45836B856EB109E6, 0x8C06046E3A7012A3 };

// formats
const KV3ID_t g_KV3Format_Generic = { "generic", 0x469806E97412167C, 0xE73790B53EE6F2AF };

class KeyValues
{
public:
	std::byte pad01[0x100];
	std::uint64_t uKey;
	void* pValue;
	std::byte pad02[0x8];

	__declspec(noinline) static bool LoadKV3(KeyValues* kv, const char* material_vmat, const char* kv_name)
	{
		using fn = bool(__fastcall*)(void* thisptr, void* utlstring, const char* buffer, const KV3ID_t* format, const char* kv_name);
		const auto load_kv3 = modules::tier0.get_export("?LoadKV3@@YA_NPEAVKeyValues3@@PEAVCUtlString@@PEBDAEBUKV3ID_t@@2@Z").as<fn>();
		if (load_kv3)
			return load_kv3(kv, nullptr, material_vmat, &g_KV3Format_Generic, kv_name);

		return false;
	}
};

class IMaterial
{
public:
	const char* GetName()
	{
		return VTable::GetThiscall<const char*>(this, 0); //working
	}

	const char* GetShareName()
	{
		return VTable::GetThiscall<const char*>(this, 1); //working aswell
	}
};

class CMaterialSystem
{
public:
	IMaterial* FindMaterial(IMaterial*** material, const char* name) //working fine
	{
		return VTable::GetThiscall<IMaterial*>(this, 14, material, name);
	}

	IMaterial* CreateMaterial(IMaterial*** material, const char* mat_name, KeyValues* key_value, uint32_t num1, uint8_t num2)
	{
		return VTable::GetThiscall<IMaterial*>(this, 29, material, mat_name, key_value, num1, num2);
	}
};

class CMaterialDrawDescription
{
public:

};

class CSceneObject
{
public:
	char pad_01[0xB8]; //0x0
	std::array<uint8_t, 4> color; //0xB8
	CHandle owner_handle; //0xBC
}; //Size: 0x0180

class CSceneData
{
public:
	std::byte pad01[0x8];                // 0x0
	CMaterialDrawDescription* mat_desc;	 // 0x8
	CHandle handle;                      // 0x10
	std::byte pad02[0x4];                // 0x14
	CSceneObject* scene_object;          // 0x18
	IMaterial* material;                 // 0x20, - material2 at 0x28
	std::byte pad04[0x28];               // 0x28
	std::array<uint8_t, 4> color;      // 0x50
}; // Size: 0x58

namespace interfaces //move to interfaces.h ?
{
	extern IVEngineCVar* cvar;
	extern IVEngineClient* engine_client;
	extern CSource2Client* client;
	extern CSchemaSystem* schema_system;
	extern IMemAlloc* mem_alloc;
	extern CGameResourceService* game_resource_service;
	extern CEntitySystem* entity_system;
	extern CSGOInput* csgo_input;
	extern CRenderSystem* render_system;
	extern CInputSystem* input_system;
	extern CClientModeCSNormal* client_mode_csnormal;
    extern CGlobalVarsBase* global_vars;
	extern CGameTrace* engine_trace;
	extern CNetworkGameService* network_game_service;
	extern CGameType* game_type;
	extern CHudChat* hud_chat;
	extern CGameRules* game_rules;
	extern CMaterialSystem* mat_system;
}

namespace g = interfaces;
