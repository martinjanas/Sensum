#include "sdk.h"
#include "../sdk/helpers/modules.h"
#include "../hooks/hooks.h"

#include <d3d11.h>

#include "../sdk/localplayer.h"

#include "../interfaces.h"

void print_status(const char* name, void* ptr)
{
	auto remove_leading_zeros = [](const std::string& input) -> std::string 
	{
		size_t start = input.find_first_not_of("0x");
		if (start != std::string::npos) 
		{
			return "0x" + input.substr(start);
		}

		return "0x0";
	};

	std::string ptr_str = std::format("{:p}", ptr);

	std::string str = std::format("{:s}: {:s}\n", name, remove_leading_zeros(ptr_str));

	printf(str.c_str());
}

#define print_status(name) print_status(#name, name)

namespace sdk
{
	extern bool is_aimkey_down = false;

	void init_modules()
	{
		modules::client = DynamicModule("client.dll");
		modules::nav_system = DynamicModule("navsystem.dll");
		modules::engine = DynamicModule("engine2.dll");
		modules::schema = DynamicModule("schemasystem.dll");
		modules::tier0 = DynamicModule("tier0.dll");
		modules::render_dx11 = DynamicModule("rendersystemdx11.dll");
		modules::input_sys = DynamicModule("inputsystem.dll");

	}

	void init_interfaces()
	{
		g::engine_client = modules::engine.GetInterfaceFromList<IVEngineClient*>("Source2EngineToClient001");
		g::client = modules::client.GetInterfaceFromList<CSource2Client*>("Source2Client002");
		g::schema_system = modules::schema.GetInterfaceFromList<CSchemaSystem*>("SchemaSystem_001");
		g::game_resource_service = modules::engine.GetInterfaceFromList<CGameResourceService*>("GameResourceServiceClientV001");
		g::input_system = modules::input_sys.GetInterfaceFromList<CInputSystem*>("InputSystemVersion001");

		g::csgo_input = g::csgo_input->get();
		g::entity_system = g::game_resource_service->GetGameEntitySystem();
		g::render_system = **reinterpret_cast<CRenderSystem***>(modules::render_dx11.pattern_scanner.scan("66 0F 7F 0D ? ? ? ? 66 0F 7F 05 ? ? ? ? 0F 1F 40", "GetRenderSystemDX11").add(4).abs().as<uint8_t*>());

		g::global_vars = *modules::client.pattern_scanner.scan("48 89 15 ?? ?? ?? ?? 48 8D 05 ?? ?? ?? ?? 48 85 D2", "GlobalVars").add(0x3).abs().as<CGlobalVarsBase**>();

		if (g::render_system)
			g::swap_chain = g::render_system->swap_chain;

		g::client_mode = modules::client.pattern_scanner.scan("48 8D 3D ? ? ? ? 48 8D 35 ? ? ? ? 90").add(0x3).abs().as();

		g::mem_alloc = modules::tier0.exporter.get_export("g_pMemAlloc").as<IMemAlloc>();
		mem_alloc_in::mem_alloc = modules::tier0.exporter.get_export("g_pMemAlloc").as<IMemAlloc>();

		print_status(g::engine_client);
		print_status(g::client);
		print_status(g::schema_system);
		print_status(g::mem_alloc);
		print_status(mem_alloc_in::mem_alloc);
		print_status(g::game_resource_service);
		print_status(g::entity_system);
		print_status(g::csgo_input);
		print_status(g::render_system);
		print_status(g::swap_chain);
		print_status(g::input_system);
		print_status(g::client_mode);
		print_status(g::global_vars);
	}
}

namespace interfaces
{
	IVEngineClient* engine_client{};
	CSource2Client* client{};
	CSchemaSystem* schema_system{};
	IMemAlloc* mem_alloc{};
	CGameResourceService* game_resource_service{};
	CGameEntitySystem* entity_system{};
	CSGOInput* csgo_input{};
	CRenderSystem* render_system{};
	IDXGISwapChain* swap_chain{};
	CInputSystem* input_system{};
	void* client_mode{};
	CGlobalVarsBase* global_vars{};
}

