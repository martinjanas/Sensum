#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include "../sdk/interfaces/IVEngineClient.h"
#include "../sdk/interfaces/SchemaSystem.h"
#include "../sdk/interfaces/IMemAlloc.h"
#include "../sdk/interfaces/CGameResourceService.h"
#include "../sdk/interfaces/CSGOInput.h"
#include "../sdk/interfaces/CSource2Client.h"
#include "../sdk/interfaces/CInputSystem.h"
#include "../sdk/math/Viewmatrix.h"

class CGlobalVars
{
	char pad_0000[8]; //0x0000 //New class
	float interval_per_ticks; //0x0008
	float curtime; //0x000C
	float curtime2; //0x0010
	char pad_0014[4]; //0x0014
	float fraction; //0x0018
	bool N00000CD9; //0x001C
	char pad_001D[3]; //0x001D
	int64_t tick_count; //0x0020
	void* net_channel; //0x0028
};

class CGlobalVarsBase
{
public:
	float realtime; //0x0000
	int32_t frame_count; //0x0004
	float interval_per_ticks; //0x0008
	float interval_per_ticks2; //0x000C
	int32_t max_client; //0x0010
	bool N00000CE3; //0x0014
	char pad_0015[3]; //0x0015
	bool prediction; //0x0018
	char pad_0019[7]; //0x0019
	CGlobalVars* global_vars;
};

class CNetworkGameClient
{
public:
	CGlobalVarsBase* GetGlobalVars()
	{
		//xref: Curtime( %f )\nRealtime( %f )\n module: engine2
		return GetVirtual<CGlobalVarsBase*(__thiscall*)(void*)>(this, 4)(this);
	}
};

class CNetworkGameService
{
public:
	CNetworkGameClient* GetNetworkGameClient()
	{
		return *reinterpret_cast<CNetworkGameClient**>(reinterpret_cast<uintptr_t>(this) + 0xB8);
	}
};

namespace sdk
{
	void init_modules();
	void init_interfaces();

	extern bool is_aimkey_down;
}

class CRenderSystem //TODO: Move this into its own file
{
public:
	std::byte pad001[0x170];
	IDXGISwapChain* swap_chain;
};

namespace interfaces //move to interfaces.h ?
{
	extern IVEngineClient* engine_client;
	extern CSource2Client* client;
	extern CSchemaSystem* schema_system;
	extern IMemAlloc* mem_alloc;
	extern CGameResourceService* game_resource_service;
	extern CGameEntitySystem* entity_system;
	extern CSGOInput* csgo_input;
	extern CRenderSystem* render_system;
	extern IDXGISwapChain* swap_chain;
	extern CInputSystem* input_system;
	extern void* client_mode;
    extern CGlobalVarsBase* global_vars;
	extern CNetworkGameService* network_game_service;
	extern CNetworkGameClient* network_game_client;
}

namespace g = interfaces;
