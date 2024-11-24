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
#include "../sdk/math/Viewmatrix.h"

namespace sdk
{
	void init_modules();
	void init_interfaces();
	void validate_interfaces_on_map_load();
	void invalidate_interfaces_on_map_unload();
	void scan_and_cache_sigs();
}

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
	extern CGameTrace* engine_trace;
	extern CNetworkGameService* network_game_service;
	extern CGameType* game_type;
	extern CHudChat* hud_chat;
	extern CGameRules* game_rules;
}

namespace g = interfaces;
