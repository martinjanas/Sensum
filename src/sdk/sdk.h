#pragma once
#include <dxgi.h>
#include <d3d11.h>
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
#include "../sdk/math/Viewmatrix.h"

namespace sdk
{
	void init_modules();
	void init_interfaces();
	void validate_interfaces_on_map_load();
	void invalidate_interfaces_on_map_unload();
	void scan_and_cache_sigs();
}

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
};

class CMaterialDrawDescription
{
public:

};

class CSceneData
{
public:
	std::byte pad01[0x8]; //0x0
	CMaterialDrawDescription* mat_desc; //0x8
	std::byte pad02[0x4]; //0xC
	CHandle handle; //0x10
	std::byte pad03[0x8]; //0x18
	IMaterial* material; //0x20
	std::byte pad04[0x18]; //0x38
	std::array<byte, 4> color; //0x40
}; //0x47

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
