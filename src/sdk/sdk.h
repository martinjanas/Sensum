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
    float m_realtime; //0x0000
    __int32 m_framecount; //0x0004
    float m_frametime; //0x0008
    float m_abs_frametime; //0x000C
    __int32 m_maxclients; //0x0010
    char pad_0014[28]; //0x0014
    float m_frametime2; //0x0030
    float m_curtime; //0x0034
    float m_curtime2; //0x0038
    char pad_003C[20]; //0x003C
    __int32 m_tickcount; //0x0050
    char pad_0054[348]; //0x0054
    DWORD64 m_current_map; //0x01B0
    DWORD64 m_current_mapname; //0x01B8
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
