#include "interfaces.h"
#include "../../cheat.h"
#include "../../helpers/modules.h"

void Interfaces::init()
{
	auto _engine_client = modules::engine.get_interface_from_list<void*>("Source2EngineToClient001");
	engine = std::make_unique<Engine>(_engine_client, "engine");
	
	auto _client = modules::client.get_interface_from_list<void*>("Source2Client002");
	client = std::make_unique<CSource2Client>(_client, "client");
	
	auto _schema_system = modules::schema.get_interface_from_list<void*>("SchemaSystem_001");
	schema_system = std::make_unique<SchemaSystemInterface>(_schema_system, "schema_system");
	
	auto _game_resource_service = modules::engine.get_interface_from_list<void*>("GameResourceServiceClientV001");
	game_resources = std::make_unique<CGameResourceService>(_game_resource_service, "game_services");

	auto _entity_system = game_resources->GetEntitySystem();
	entity_system = std::make_unique<CEntitySystem>(_entity_system, "entity_system");
	
	auto _input_system = modules::input_sys.get_interface_from_list<void*>("InputSystemVersion001");
	input_system = std::make_unique<CInputSystem>(_input_system, "input_system");
	
	auto _network_game_service = modules::engine.get_interface_from_list<void*>("NetworkClientService_001");
	network_service = std::make_unique<CNetworkGameService>(_network_game_service, "network_service");
	
	auto _game_type = modules::matchmaking.get_interface_from_list<void*>("GameTypes001");
	game_type = std::make_unique<CGameType>(_game_type, "game_type");
	
	auto _cvar = modules::tier0.get_interface_from_list<void*>("VEngineCvar007");
	cvar = std::make_unique<CVarInterface>(_cvar, "cvar");
	
	auto _engine_trace = *modules::client.get_sig(xxh::get_hash("g::engine_trace")).as<void**>();
	engine_trace = std::make_unique<CGameTrace>(_engine_trace, "engine_trace");
	
	auto _client_mode_csnormal = modules::client.get_sig(xxh::get_hash("g::clientmode_csnormal")).as<void*>();
	client_mode = std::make_unique<CClientModeCSNormal>(_client_mode_csnormal, "client_mode");
	
	auto _game_rules = *modules::client.get_sig(xxh::get_hash("g::game_rules")).as<void**>();
	game_rules = std::make_unique<CGameRules>(_game_rules, "game_rules");
	
	auto _mat_system = modules::materialsys.get_interface_from_list<void*>("VMaterialSystem2_001");
	material_system = std::make_unique<CMaterialSystem>(_mat_system, "mat_system");
	
	auto _mem_alloc = modules::tier0.get_export("g_pMemAlloc").as();
	mem_alloc = std::make_unique<IMemAlloc>(_mem_alloc, "mem_alloc");
	
	//auto _hud_chat = *reinterpret_cast<CHudChat**>(globals::find_hud_element("HudChatDelegate"));
	
	{
		csgo_input = modules::client.get_sig(xxh::get_hash("g::csgo_input")).as<CSGOInput*>();
		cheat::console().println("csgo_input: 0x%p", csgo_input);
	
		render_system = **modules::directx11.get_sig(xxh::get_hash("g::render_system")).as<CRenderSystem***>();
		cheat::console().println("render_system: 0x%p", render_system);

		global_vars = *modules::client.get_sig(xxh::get_hash("g::global_vars")).as<CGlobalVarsBase**>();
		cheat::console().println("global_vars: 0x%p", global_vars);
	}
	
	m_interfaces.emplace_back(std::ref(*engine));
	m_interfaces.emplace_back(std::ref(*mem_alloc));
}

void Interfaces::print_status() const
{
	for (const auto& x : m_interfaces)
		x.get().print();
}

void Interfaces::validate_interfaces_on_map_load()
{
	global_vars = *modules::client.get_sig(xxh::get_hash("g::global_vars")).as<CGlobalVarsBase**>();
	cheat::console().println("global_vars: 0x%p", global_vars);
        
	auto _game_rules = *modules::client.get_sig(xxh::get_hash("g::game_rules")).as<void**>();
	game_rules = std::make_unique<CGameRules>(_game_rules, "game_rules");
}

void Interfaces::invalidate_interfaces()
{
	global_vars = nullptr;
	game_rules = nullptr;
}
