#include "sdk.h"
#include "../sdk/helpers/modules.h"
#include "../hooks/hooks.h"

#include <d3d11.h>

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
	void init_modules()
	{
		modules::client = DynamicModule("client.dll");
		modules::nav_system = DynamicModule("navsystem.dll");
		modules::engine = DynamicModule("engine2.dll");
		modules::schema = DynamicModule("schemasystem.dll");
		modules::tier0 = DynamicModule("tier0.dll");
		modules::render_dx11 = DynamicModule("rendersystemdx11.dll");

	}

	void init_interfaces()
	{
		g::engine_client = modules::engine.GetInterfaceFromList<IVEngineClient*>("Source2EngineToClient001");
		g::schema_system = modules::schema.GetInterfaceFromList<CSchemaSystem*>("SchemaSystem_001");
		g::game_resource_service = modules::engine.GetInterfaceFromList<CGameResourceService*>("GameResourceServiceClientV001");

		g::csgo_input = g::csgo_input->get();
		g::entity_system = g::game_resource_service->GetGameEntitySystem();
		g::render_system = **reinterpret_cast<CRenderSystem***>(modules::render_dx11.pattern_scanner.scan("66 0F 7F 05 ? ? ? ? 66 0F 7F 0D ? ? ? ? 48 89 35 ? ? ? ?").add(4).abs().as<uint8_t>());

		if (g::render_system)
			g::swap_chain = g::render_system->swap_chain;

		g::mem_alloc = *reinterpret_cast<IMemAlloc**>(modules::tier0.GetExport("g_pMemAlloc"));

		print_status(g::engine_client);
		print_status(g::schema_system);
		print_status(g::mem_alloc);
		print_status(g::game_resource_service);
		print_status(g::entity_system);
		print_status(g::csgo_input);
		print_status(g::render_system);
		print_status(g::swap_chain);
	}
}

namespace interfaces
{
	IVEngineClient* engine_client{};
	CSchemaSystem* schema_system{};
	IMemAlloc* mem_alloc{};
	CGameResourceService* game_resource_service{};
	CGameEntitySystem* entity_system{};
	CSGOInput* csgo_input{};
	CRenderSystem* render_system{};
	IDXGISwapChain* swap_chain{};
}

