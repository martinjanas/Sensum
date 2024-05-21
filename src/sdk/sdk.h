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
}

namespace g = interfaces;
