#include "sdk.hpp"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../valve_sdk/interfaces/CGlowObjectManager.h"

#include <optional>

#define STRINGIFY_IMPL(s) #s
#define STRINGIFY(s)      STRINGIFY_IMPL(s)
#define PRINT_INTERFACE(name) console::print("%-20s: %p", STRINGIFY(name), name)

namespace g
{
	CLocalPlayer local_player;

	CInput* input = nullptr;
	IMDLCache* mdl_cache = nullptr;
	ICvar* cvar = nullptr;
	IPanel* vgui_panel = nullptr;
	IVModelInfo* mdl_info = nullptr;
	ISurface* surface = nullptr;
	IClientMode* client_mode = nullptr;
	IMoveHelper* move_helper = nullptr;
	IPrediction* prediction = nullptr;
	IViewRender* view_render = nullptr;
	IVModelRender* mdl_render = nullptr;
	IVRenderView* render_view = nullptr;
	IEngineTrace* engine_trace = nullptr;
	IEngineSound* engine_sound = nullptr;
	CClientState* client_state = nullptr;
	IInputSystem* input_system = nullptr;
	IVDebugOverlay* debug_overlay = nullptr;
	IGameEventManager2* game_events = nullptr;
	IMaterialSystem* mat_system = nullptr;
	IBaseClientDLL* base_client = nullptr;
	CGameMovement* game_movement = nullptr;
	IVEngineClient* engine_client = nullptr;
	CGlobalVarsBase* global_vars = nullptr;
	IDirect3DDevice9* d3_device = nullptr;
	IClientEntityList* entity_list = nullptr;
	IPhysicsSurfaceProps* physics_surface = nullptr;
	IVEngineVGui* engine_vgui = nullptr;
	CRender* render = nullptr;
	c_cs_player_resource** player_resource = nullptr;
	CHud* hud_system = nullptr;
	ILocalize* localize = nullptr;
	IMemAlloc* mem_alloc = nullptr;
	IWeaponSystem* weapon_system = nullptr;
	IFileSystem* file_system = nullptr;
	IViewRenderBeams* view_render_beams = nullptr;
	glow_manager_t* glow_manager = nullptr;
	IStudioRender* g_studiorender = nullptr;
	IDemoPlayer* demo_player = nullptr;

	ISteamUser* steam_user = nullptr;
	ISteamHTTP* steam_http = nullptr;
	ISteamClient* steam_client = nullptr;
	ISteamFriends* steam_friends = nullptr;
	ISteamGameCoordinator* game_coordinator = nullptr;

	CHudChat* hud_chat = nullptr;
	C_TEFireBullets* fire_bullets = nullptr;
	c_cs_game_rules_proxy* game_rules_proxy = nullptr;

	CreateInterfaceFn get_module_factory(HMODULE module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
	}

	template<typename T>
	T* get_interface(const char* moduleName, const char* szInterfaceVersion)
	{
		auto result = reinterpret_cast<T*>(get_module_factory(utils::get_module(moduleName))(szInterfaceVersion, nullptr));

		if (!result) {
			throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
		}

		return result;
	}

	template <typename T>
	T get_steam_interface(const char* version)
	{
		static const auto steam_api = utils::get_module("steam_api.dll");
		return reinterpret_cast<T(__cdecl*)(void)>(GetProcAddress(steam_api, version))();
	}

	void initialize()
	{
		global_vars = **(CGlobalVarsBase***)(utils::pattern_scan(GLOBAL_VARS) + 1);
		client_mode = *(IClientMode**)(utils::pattern_scan(CLIENT_MODE) + 1);
		input = *(CInput**)(utils::pattern_scan(CINPUT) + 1);
		move_helper = **(IMoveHelper***)(utils::pattern_scan(MOVE_HELPER) + 2);
		view_render = *(IViewRender**)(utils::pattern_scan(VIEW_RENDER) + 1);
		d3_device = **(IDirect3DDevice9***)(utils::pattern_scan(D3D_DEVICE) + 1);
		client_state = **(CClientState***)(utils::pattern_scan(CLIENT_STATE) + 1);
		local_player = *(CLocalPlayer*)(utils::pattern_scan(LOCAL_PLAYER) + 2);
		render = *(CRender**)(utils::pattern_scan(CRENDER) + 7);
		hud_system = *reinterpret_cast<CHud**>(utils::pattern_scan(CHUD) + 1);
		player_resource = *reinterpret_cast<c_cs_player_resource***>(utils::pattern_scan(PLAYER_RESOURCE) + 2);
		weapon_system = *(IWeaponSystem**)(utils::pattern_scan(WEAPON_SYSTEM) + 2);
		view_render_beams = *(IViewRenderBeams**)(utils::pattern_scan(VIEW_RENDER_BEAMS) + 1);
		fire_bullets = *(C_TEFireBullets**)(utils::pattern_scan(FIRE_BULLETS) + 0x131);
		game_rules_proxy = **(c_cs_game_rules_proxy***)(utils::pattern_scan(GAME_RULES_PROXY) + 1);
		glow_manager = (glow_manager_t*)(*(uintptr_t*)(utils::pattern_scan(GetModuleHandleA("client.dll"), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3));
		demo_player = **reinterpret_cast<IDemoPlayer***>(utils::pattern_scan("engine.dll", "8B 0D ? ? ? ? 8B 01 8B 40 1C FF D0 84 C0 74 0A") + 0x2);

		base_client = get_interface<IBaseClientDLL>("client.dll", "VClient018");
		entity_list = get_interface<IClientEntityList>("client.dll", "VClientEntityList003");
		prediction = get_interface<IPrediction>("client.dll", "VClientPrediction001");
		game_movement = get_interface<CGameMovement>("client.dll", "GameMovement001");
		mdl_cache = get_interface<IMDLCache>("datacache.dll", "MDLCache004");
		engine_client = get_interface<IVEngineClient>("engine.dll", "VEngineClient014");
		mdl_info = get_interface<IVModelInfo>("engine.dll", "VModelInfoClient004");
		mdl_render = get_interface<IVModelRender>("engine.dll", "VEngineModel016");
		render_view = get_interface<IVRenderView>("engine.dll", "VEngineRenderView014");
		engine_trace = get_interface<IEngineTrace>("engine.dll", "EngineTraceClient004");
		debug_overlay = get_interface<IVDebugOverlay>("engine.dll", "VDebugOverlay004");
		game_events = get_interface<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");
		mat_system = get_interface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
		cvar = get_interface<ICvar>("vstdlib.dll", "VEngineCvar007");
		vgui_panel = get_interface<IPanel>("vgui2.dll", "VGUI_Panel009");
		surface = get_interface<ISurface>("vguimatsurface.dll", "VGUI_Surface031");
		physics_surface = get_interface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps001");
		engine_sound = get_interface<IEngineSound>("engine.dll", "IEngineSoundClient003");
		engine_vgui = get_interface<IVEngineVGui>("engine.dll", "VEngineVGui001");
		input_system = get_interface<IInputSystem>("inputsystem.dll", "InputSystemVersion001");
		localize = get_interface<ILocalize>("localize.dll", "Localize_001");
		file_system = get_interface<IFileSystem>("filesystem_stdio.dll", "VFileSystem017");
		g_studiorender = get_interface<IStudioRender>("studiorender.dll", "VStudioRender026");

		mem_alloc = *(IMemAlloc**)GetProcAddress(utils::get_module("tier0.dll"), "g_pMemAlloc");

		const auto _steam_user = get_steam_interface<HSteamUser>("SteamAPI_GetHSteamUser");
		const auto _steam_pipe = get_steam_interface<HSteamPipe>("SteamAPI_GetHSteamPipe");

		steam_client = get_steam_interface<ISteamClient*>("SteamClient");
		steam_user = steam_client->GetISteamUser(_steam_user, _steam_user, "SteamUser019");
		game_coordinator = static_cast<ISteamGameCoordinator*>(steam_client->GetISteamGenericInterface(_steam_user, _steam_pipe, "SteamGameCoordinator001"));
		steam_friends = steam_client->GetISteamFriends(_steam_user, _steam_pipe, "SteamFriends015");
		steam_http = steam_client->GetISteamHTTP(_steam_user, _steam_pipe, "STEAMHTTP_INTERFACE_VERSION002");

		hud_chat = hud_system->FindHudElement<CHudChat>("CHudChat");

#ifdef _DEBUG
		PRINT_INTERFACE(global_vars);
		PRINT_INTERFACE(client_mode);
		PRINT_INTERFACE(input);
		PRINT_INTERFACE(move_helper);
		PRINT_INTERFACE(view_render);
		PRINT_INTERFACE(d3_device);
		PRINT_INTERFACE(client_state);
		PRINT_INTERFACE(base_client);
		PRINT_INTERFACE(entity_list);
		PRINT_INTERFACE(prediction);
		PRINT_INTERFACE(game_movement);
		PRINT_INTERFACE(mdl_cache);
		PRINT_INTERFACE(engine_client);
		PRINT_INTERFACE(mdl_info);
		PRINT_INTERFACE(mdl_render);
		PRINT_INTERFACE(render_view);
		PRINT_INTERFACE(engine_trace);
		PRINT_INTERFACE(debug_overlay);
		PRINT_INTERFACE(game_events);
		PRINT_INTERFACE(mat_system);
		PRINT_INTERFACE(cvar);
		PRINT_INTERFACE(vgui_panel);
		PRINT_INTERFACE(surface);
		PRINT_INTERFACE(physics_surface);
		PRINT_INTERFACE(local_player);
		PRINT_INTERFACE(game_coordinator);
		PRINT_INTERFACE(steam_client);
		PRINT_INTERFACE(steam_user);
		PRINT_INTERFACE(engine_sound);
		PRINT_INTERFACE(input_system);
		PRINT_INTERFACE(steam_friends);
		PRINT_INTERFACE(steam_http);
		PRINT_INTERFACE(engine_vgui);
		PRINT_INTERFACE(render);
		PRINT_INTERFACE(player_resource);
		PRINT_INTERFACE(hud_system);
		PRINT_INTERFACE(g_studiorender);
		PRINT_INTERFACE(localize);
		PRINT_INTERFACE(mem_alloc);
		PRINT_INTERFACE(weapon_system);
		PRINT_INTERFACE(file_system);
		PRINT_INTERFACE(view_render_beams);
		PRINT_INTERFACE(hud_chat);
		PRINT_INTERFACE(demo_player);
#endif
	}
}