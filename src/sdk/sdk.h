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

class CGlobalVarsBase 
{
public:
	float realtime; //0x0000
	int framecount; //0x0004
	float absoluteframetime; //0x0008
	float absoluteframestarttimestddev; //0x000C
	int max_clients; //0x0010
	float interval_per_tick; //0x0014
	char pad_0018[8]; //0x0018
	void* get_curtime; //0x0020
	float frametime; //0x0028
	float curtime; //0x002C
	float curtime2; //0x0030
	char pad_0034[16]; //0x0034
	int tickcount; //0x0044
	void* net_channel; //0x0048
	char pad_0050[232]; //0x0050
	void* network_string_table_container; //0x0138
	char pad_0140[64]; //0x0140
	char* current_map; //0x0180
	char* current_mapname; //0x0188
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
}

namespace g = interfaces;
