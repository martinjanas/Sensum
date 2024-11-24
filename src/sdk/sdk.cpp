#include "sdk.h"
#include "../sdk/helpers/modules.h"
#include "../hooks/hooks.h"

#include <d3d11.h>
#include <future>

#include "../sdk/helpers/interfaces.h"

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

	std::string str = std::format("{:s}: {:s}", name, remove_leading_zeros(ptr_str));

	g_Console->println(str.c_str());
}

#define print_status(name) print_status(#name, name)

namespace sdk
{
	void scan_and_cache_sigs()
	{
		std::vector<std::future<void>> futures;

		static auto scan_and_cache_sig = [&](DynamicModule* module, const std::string_view& sig, const std::string_view& name, const uintptr_t& offset, bool abs)
		{
			module->scan_and_cache_sig(sig, name, offset, abs);
		};

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 8B 05 ? ? ? ? 48 85 C0 74 53", "g::localplayer", 0x3, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 89 05 ? ? ? ? 0F 57 C0 0F 11 05 ? ? ? ?", "g::csgo_input", 0x3, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::directx11, "66 0F 7F 0D ? ? ? ? 48 8B F7 66 0F 7F 05", "g::render_system", 0x4, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 8B 05 ?? ?? ?? ?? 44 8B B7 ?? ?? ?? ?? 8B 70 04 B8 ?? ?? ?? ??", "g::global_vars", 0x3, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 8B 0D ? ? ? ? 4C 8B C3 66 89 44 24", "g::engine_trace", 0x3, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 8D 3D ? ? ? ? 48 8D 35 ? ? ? ? 90", "g::clientmode_csnormal", 0x3, true));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 89 5C 24 08 48 89 74 24 10 57 48 81 EC 40 01 00 00 8B DA 48 8B F9 E8 ?? ?? ?? ??", "CBaseEntity::GetHitboxSet", 0, false));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 89 5C 24 ? 55 57 41 54 41 56 41 57 48 83 EC 20", "CBaseEntity::HitboxToWorldTransform", 0, false));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 8B C4 48 89 58 10 48 89 70 20 55 57 41 56 48 8D A8 08", "CBaseEntity::EmitSound", 0, false));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "85 D2 78 16 3B 91", "CModel::GetBoneFlags", 0, false));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "E8 ? ? ? ? 41 0F 10 14 3F", "CModel::GetBoneParent", 0x1, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "85 D2 78 25 3B 91", "CModel::GetBoneName", 0, false));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "40 55 48 83 EC 20 48 83 3D ? ? ? ? ?", "globals::FindHudElement", 0, false));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 89 5C 24 20 48 89 4C 24 08 55 56 41", "CGameTrace::TraceShape", 0, false));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 54 41 56 41 57 48 81 EC C0 00 00 00 48 8B 9C", "CGameTrace::ClipRayToEntity", 0, false));
		
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "E8 ? ? ? ? 48 81 4B ? ? ? ? ? 48 8D 05 ? ? ? ?", "TraceFilter_t::InitEntitiesOnly", 0x1, true));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "E8 ? ? ? ? 0F 28 F8 44 0F 28 54 24 ?", "IsInSmoke", 0x1, true));

		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "E8 ? ? ? ? F3 0F 11 45 ? 48 8B 5C 24 ?", "hooks::GetFov", 0x1, true));
		futures.push_back(std::async(std::launch::async, scan_and_cache_sig, &modules::client, "40 53 48 81 EC ? ? ? ? 49 8B C1", "hooks::GetMatricesForView", 0, false));

		for (auto& future : futures)
			future.get();
	}

	void init_modules()
	{
		modules::client = DynamicModule("client.dll");
		modules::nav_system = DynamicModule("navsystem.dll");
		modules::engine = DynamicModule("engine2.dll");
		modules::schema = DynamicModule("schemasystem.dll");
		modules::tier0 = DynamicModule("tier0.dll");
		modules::directx11 = DynamicModule("rendersystemdx11.dll");
		modules::input_sys = DynamicModule("inputsystem.dll");
		modules::matchmaking = DynamicModule("matchmaking.dll");
		modules::gameoverlay = DynamicModule("GameOverlayRenderer64.dll");

		scan_and_cache_sigs();
	}

	void init_interfaces()
	{
		g::engine_client = modules::engine.get_interface_from_list<IVEngineClient*>("Source2EngineToClient001");
		g::client = modules::client.get_interface_from_list<CSource2Client*>("Source2Client002");
		g::schema_system = modules::schema.get_interface_from_list<CSchemaSystem*>("SchemaSystem_001");
		g::game_resource_service = modules::engine.get_interface_from_list<CGameResourceService*>("GameResourceServiceClientV001");
		g::input_system = modules::input_sys.get_interface_from_list<CInputSystem*>("InputSystemVersion001");
		g::network_game_service = modules::engine.get_interface_from_list<CNetworkGameService*>("NetworkClientService_001");
		g::game_type = modules::matchmaking.get_interface_from_list<CGameType*>("GameTypes001");

		g::csgo_input = modules::client.get_sig_addr(FNV("g::csgo_input")).as<CSGOInput*>();
		g::render_system = **modules::directx11.get_sig_addr(FNV("g::render_system")).as<CRenderSystem***>();
		g::global_vars = *modules::client.get_sig_addr(FNV("g::global_vars")).as<CGlobalVarsBase**>();
		g::engine_trace = *modules::client.get_sig_addr(FNV("g::engine_trace")).as<CGameTrace**>();
		g::client_mode_csnormal = modules::client.get_sig_addr(FNV("g::clientmode_csnormal")).as<CClientModeCSNormal*>();
		g::localplayer = modules::client.get_sig_addr(FNV("g::localplayer")).as<CCSPlayerController*>();

		g::entity_system = g::game_resource_service->GetEntitySystem();

		g::mem_alloc = modules::tier0.get_export("g_pMemAlloc").as<IMemAlloc*>();
		mem_alloc_in::mem_alloc = modules::tier0.get_export("g_pMemAlloc").as<IMemAlloc*>();

		print_status(g::localplayer);
		print_status(g::engine_client);
		print_status(g::client);
		print_status(g::schema_system);
		print_status(g::mem_alloc);
		print_status(g::game_resource_service);
		print_status(g::entity_system);
		print_status(g::csgo_input);
		print_status(g::render_system);
		print_status(g::input_system);
		print_status(g::client_mode_csnormal);
		print_status(g::global_vars);
		print_status(g::engine_trace);
		print_status(g::network_game_service);
		//print_status(g::hud_chat);
	}
}

namespace interfaces
{
	CCSPlayerController* localplayer{};
	IVEngineClient* engine_client{};
	CSource2Client* client{};
	CSchemaSystem* schema_system{};
	IMemAlloc* mem_alloc{};
	CGameResourceService* game_resource_service{};
	CEntitySystem* entity_system{};
	CSGOInput* csgo_input{};
	CRenderSystem* render_system{};
	CInputSystem* input_system{};
	CClientModeCSNormal* client_mode_csnormal{};
	CGlobalVarsBase* global_vars{};
	CGameTrace* engine_trace{};
	CNetworkGameService* network_game_service{};
	CGameType* game_type{};
	CHudChat* hud_chat{};
}

