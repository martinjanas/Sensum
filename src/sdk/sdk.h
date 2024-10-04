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

#include "../sdk/math/Viewmatrix.h"
#include "../sdk/helpers/console.h"

class CGlobalVarsBase
{
public:
	float m_realtime; //0x0000
	int32_t m_framecount; //0x0004
	float m_fratmetime; //0x0008
	float m_abs_frametime; //0x000C
	int32_t m_maxclients; //0x0010
	char pad_0014[28]; //0x0014
	float m_frametime2; //0x0030
	float m_curtime; //0x0034
	float m_curtime2; //0x0038
	char pad_003C[20]; //0x003C
	int32_t m_tickcount; //0x0050
	char pad_0054[292]; //0x0054
	uint64_t m_current_map; //0x0178
	uint64_t m_current_mapname; //0x0180
}; //Size: 0x0188

class CClientModeCSNormal
{
public:

};

//class CNetworkGameClient //implement these later:
//{
//public:
//	CGlobalVarsBase* GetGlobalVars()
//	{
//		//xref: Curtime( %f )\nRealtime( %f )\n module: engine2
//		return GetVirtual<CGlobalVarsBase * (__thiscall*)(void*)>(this, 4)(this);
//	}
//};

//g::network_game_service = modules::engine.GetInterfaceFromList<CNetworkGameService*>("NetworkClientService_001");
//class CNetworkGameService
//{
//public:
//	CNetworkGameClient* GetNetworkGameClient()
//	{
//		return *reinterpret_cast<CNetworkGameClient**>(reinterpret_cast<uintptr_t>(this) + 0xB8);
//	}
//};

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
	extern CEntitySystem* entity_system;
	extern CSGOInput* csgo_input;
	extern CRenderSystem* render_system;
	extern CInputSystem* input_system;
	extern CClientModeCSNormal* client_mode_csnormal;
    extern CGlobalVarsBase* global_vars;
	extern CGameTrace* game_trace;

	extern Console* console;
}

namespace g = interfaces;
