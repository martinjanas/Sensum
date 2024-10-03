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
		modules::directx11 = DynamicModule("rendersystemdx11.dll");
		modules::input_sys = DynamicModule("inputsystem.dll");

	}

	void init_interfaces()
	{
		g::engine_client = modules::engine.GetInterfaceFromList<IVEngineClient*>("Source2EngineToClient001");
		g::client = modules::client.GetInterfaceFromList<CSource2Client*>("Source2Client002");
		g::schema_system = modules::schema.GetInterfaceFromList<CSchemaSystem*>("SchemaSystem_001");
		g::game_resource_service = modules::engine.GetInterfaceFromList<CGameResourceService*>("GameResourceServiceClientV001");
		g::input_system = modules::input_sys.GetInterfaceFromList<CInputSystem*>("InputSystemVersion001");

		g::csgo_input = modules::client.scan("E8 ? ? ? ? 48 8B 56 60", "g::csgo_input").add(0x1).abs().as<CSGOInput*>();
		g::render_system = **modules::directx11.scan("66 0F 7F 0D ? ? ? ? 66 0F 7F 05 ? ? ? ? 48 89 2D", "g::render_system").add(4).abs().as<CRenderSystem***>();
		g::global_vars = *modules::client.scan("48 8B 05 ? ? ? ? 8B 48 04 FF C1", "g::global_vars").add(0x3).abs().as<CGlobalVarsBase**>();
		g::game_trace = *modules::client.scan("48 8B 0D ? ? ? ? 4C 8B C3 66 89 44 24", "g::game_trace").add(0x3).abs().as<CGameTrace**>();
		g::client_mode = modules::client.scan("48 8D 3D ? ? ? ? 48 8D 35 ? ? ? ? 90", "g::client_mode").add(0x3).abs().as();
		players::localplayer = modules::client.scan("48 83 3D ?? ?? ?? ?? ?? 0F 95 C0 C3", "players::localplayer").add(0x3).abs().as<CCSPlayerController*>();
		
		g::entity_system = g::game_resource_service->GetEntitySystem();

		g::mem_alloc = modules::tier0.get_export("g_pMemAlloc").as<IMemAlloc*>();
		mem_alloc_in::mem_alloc = modules::tier0.get_export("g_pMemAlloc").as<IMemAlloc*>();

		print_status(g::engine_client);
		print_status(g::client);
		print_status(g::schema_system);
		print_status(g::mem_alloc);
		print_status(g::game_resource_service);
		print_status(g::entity_system);
		print_status(g::csgo_input);
		print_status(g::render_system);
		print_status(g::input_system);
		print_status(g::client_mode);
		print_status(g::global_vars);
		print_status(g::game_trace);
		print_status(players::localplayer);
	}
}

namespace interfaces
{
	IVEngineClient* engine_client{};
	CSource2Client* client{};
	CSchemaSystem* schema_system{};
	IMemAlloc* mem_alloc{};
	CGameResourceService* game_resource_service{};
	CEntitySystem* entity_system{};
	CSGOInput* csgo_input{};
	CRenderSystem* render_system{};
	CInputSystem* input_system{};
	void* client_mode{};
	CGlobalVarsBase* global_vars{};
	CGameTrace* game_trace{};
}

