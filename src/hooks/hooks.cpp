#include "hooks.h"
#include "../settings/globals.h"
#include "../render/render.h"
#include "../helpers/utils.h"
#include "../helpers/console.h"
#include "../helpers/entities.h"
#include "../helpers/notifies.h"
#include "../features/features.h"
#include "c_game_event_listener.h"
#include "../features/Chams.h"
#include "../helpers/HitPossitionHelper.h"
#include "../helpers/runtime_saver.h"
#include "..//jsoncpp/json.h"
#include "..//helpers/input.h"

static CCSGOPlayerAnimState g_AnimState;

std::map<int, item_setting> m_items = { };

namespace hooks
{
	c_game_event_listener* event_listener;

	void init()
	{
		if (!utils::Insecure())
		{
			sequence::hook = new recv_prop_hook(c_base_view_model::m_nSequence(), sequence::hooked);
		}
		end_scene::setup = reinterpret_cast<void*>(utils::GetVirtual(g::d3_device, hooks::end_scene::index));
		create_move::setup = reinterpret_cast<void*>(utils::GetVirtual(g::client_mode, hooks::create_move::index));
		reset::setup = reinterpret_cast<void*>(utils::GetVirtual(g::d3_device, hooks::reset::index));
		paint_traverse::setup = reinterpret_cast<void*>(utils::GetVirtual(g::vgui_panel, hooks::paint_traverse::index));
		override_view::setup = reinterpret_cast<void*>(utils::GetVirtual(g::client_mode, hooks::override_view::index));
		find_mdl::setup = reinterpret_cast<void*>(utils::GetVirtual(g::mdl_cache, hooks::find_mdl::index));
		scene_end::setup = reinterpret_cast<void*>(utils::GetVirtual(g::render_view, hooks::scene_end::index));
		emit_sound::setup = reinterpret_cast<void*>(utils::GetVirtual(g::engine_sound, hooks::emit_sound::index));
		is_connected::setup = reinterpret_cast<void*>(utils::GetVirtual(g::engine_client, hooks::is_connected::index));;
		post_screen_effects::setup = reinterpret_cast<void*>(utils::GetVirtual(g::client_mode, hooks::post_screen_effects::index));
		frame_stage_notify::setup = reinterpret_cast<void*>(utils::GetVirtual(g::base_client, hooks::frame_stage_notify::index));
		draw_model_execute::setup = reinterpret_cast<void*>(utils::GetVirtual(g::mdl_render, hooks::draw_model_execute::index));
		check_file_crc_server::setup = reinterpret_cast<void*>(utils::pattern_scan(GetModuleHandleA(xorstr_("engine.dll")), xorstr_("55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80")));
		loose_file_allowed::setup = reinterpret_cast<void*>(utils::GetVirtual(g::file_system, hooks::loose_file_allowed::index));
		read_packet::setup = reinterpret_cast<void*>(utils::GetVirtual(g::demo_player, hooks::read_packet::index));
		hud_update::setup = reinterpret_cast<void*>(utils::GetVirtual(g::base_client, hooks::hud_update::index));
		is_playing_demo::setup = reinterpret_cast<void*>(utils::GetVirtual(g::engine_client, hooks::is_playing_demo::index));

		if (MH_Initialize() != MH_OK) {
			MessageBoxA(NULL, "Failed to initialize Minhook.", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(end_scene::setup, &hooks::end_scene::hooked, reinterpret_cast<void**>(&end_scene::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - DirectX: End Scene", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(create_move::setup, &hooks::create_move::hooked, reinterpret_cast<void**>(&create_move::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Create Move", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(reset::setup, &hooks::reset::hooked, reinterpret_cast<void**>(&reset::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - DirectX: Reset", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(paint_traverse::setup, &hooks::paint_traverse::hooked, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Paint Traverse", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(override_view::setup, &hooks::override_view::hooked, reinterpret_cast<void**>(&override_view::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Override View", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(is_connected::setup, &hooks::is_connected::hooked, reinterpret_cast<void**>(&is_connected::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Is Connected", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(post_screen_effects::setup, &hooks::post_screen_effects::hooked, reinterpret_cast<void**>(&post_screen_effects::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Post Screen Effects", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(emit_sound::setup, &hooks::emit_sound::hooked, reinterpret_cast<void**>(&emit_sound::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Emit Sound", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(scene_end::setup, &hooks::scene_end::hooked, reinterpret_cast<void**>(&scene_end::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Scene End", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(find_mdl::setup, &hooks::find_mdl::hooked, reinterpret_cast<void**>(&find_mdl::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Find MDL", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(frame_stage_notify::setup, &hooks::frame_stage_notify::hooked, reinterpret_cast<void**>(&frame_stage_notify::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Frame Stage Notify", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(draw_model_execute::setup, &hooks::draw_model_execute::hooked, reinterpret_cast<void**>(&draw_model_execute::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Draw Model Execute", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(check_file_crc_server::setup, &hooks::check_file_crc_server::hooked, reinterpret_cast<void**>(&check_file_crc_server::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated pattern - Check File CRC Server", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(loose_file_allowed::setup, &hooks::loose_file_allowed::hooked, reinterpret_cast<void**>(&loose_file_allowed::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Loose File Allowed", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(read_packet::setup, &hooks::read_packet::hooked, reinterpret_cast<void**>(&read_packet::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Read Packet", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(hud_update::setup, &hooks::hud_update::hooked, reinterpret_cast<void**>(&hud_update::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Hud Update", MB_OK, MB_ICONERROR);
		}

		if (MH_CreateHook(is_playing_demo::setup, &hooks::is_playing_demo::hooked, reinterpret_cast<void**>(&is_playing_demo::original)) != MH_OK) {
			MessageBoxA(NULL, "Outdated index - Is Playing Demo", MB_OK, MB_ICONERROR);
		}

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) {
			MessageBoxA(NULL, "Failed to enable hooks.", MB_OK, MB_ICONERROR);
		}

		event_listener = new c_game_event_listener();
		g::game_events->add_listener(event_listener, xorstr_("game_newmap"), false);
		g::game_events->add_listener(event_listener, xorstr_("switch_team"), false);
		g::game_events->add_listener(event_listener, xorstr_("player_hurt"), false);
		g::game_events->add_listener(event_listener, xorstr_("bullet_impact"), false);
		g::game_events->add_listener(event_listener, xorstr_("item_purchase"), false);
		g::game_events->add_listener(event_listener, xorstr_("player_spawned"), false);
		g::game_events->add_listener(event_listener, xorstr_("cs_pre_restart"), false);
		g::game_events->add_listener(event_listener, xorstr_("round_freeze_end"), false);
		g::game_events->add_listener(event_listener, xorstr_("announce_phase_end"), false);
		g::game_events->add_listener(event_listener, xorstr_("round_start"), false);
		g::game_events->add_listener(event_listener, xorstr_("player_footstep"), false);
		g::game_events->add_listener(event_listener, xorstr_("player_death"), false);

		g::cvar->ConsoleColorPrintf(Color::White, "hooks::init() Done!\n");

		//convars::sensum_i_dont_speak_nn_lang = new ConVar("sensum_i_dont_speak_nn_lang", "0", FCVAR_RELEASE, "We dont speak nn language, sorry not sorry.");
		//convars::con = new ConCommand("bhop", reinterpret_cast<FnCommandCallbackV1_t>(base), "base", FCVAR_RELEASE);

		//g::cvar->RegisterConCommand(convars::sensum_mute_russians);
		//g::cvar->RegisterConCommand(convars::sensum_i_dont_speak_nn_lang);
	}

	void destroy()
	{
		MH_Uninitialize();
		MH_DisableHook(MH_ALL_HOOKS);

		g::game_events->remove_listener(event_listener);

		if (!utils::Insecure())
			delete sequence::hook;

		//g::cvar->UnregisterConCommand(convars::sensum_mute_russians);
		//g::cvar->UnregisterConCommand(convars::sensum_i_dont_speak_nn_lang);

		//delete convars::sensum_mute_russians;
		//delete convars::sensum_i_dont_speak_nn_lang;
	}

	bool __stdcall is_playing_demo::hooked()
	{
		if (*static_cast<uintptr_t*>(_ReturnAddress()) == 0x0975C084 // client.dll : 84 C0 75 09 38 05
			&& **reinterpret_cast<uintptr_t**>(uintptr_t(_AddressOfReturnAddress()) + 4) == 0x0C75C084) { // client.dll : 84 C0 75 0C 5B
			return true;
		}
		return original();
	}

	netpacket_t* __fastcall read_packet::hooked(IDemoPlayer* player)
	{
		if (settings::misc::ow_reveal)
		{
			player->SetOverwatchState(true);

			auto ret = original(g::demo_player);

			player->SetOverwatchState(false);

			player->IsPlayingDemo() ? settings::chams::misc::dropped_weapons = false : true;


			return ret;
		}
	}

	void __fastcall hud_update::hooked(IBaseClientDLL* _this, void* edx, bool active)
	{
		if (settings::misc::ow_reveal && g::demo_player->IsPlayingDemo()) {
			g::demo_player->SetOverwatchState(true);

			original(g::base_client, active);

			g::demo_player->SetOverwatchState(false);
		}
		else {
			original(g::base_client, active);
		}
	}

	void __fastcall check_file_crc_server::hooked(void* ecx, void* edx)
	{
		return;
	}

	bool __fastcall loose_file_allowed::hooked(void* ecx, void* edx)
	{
		return true;
	}

	MDLHandle_t __fastcall find_mdl::hooked(void* ecx, void* edx, char* FilePath)
	{
		/*if (strstr(FilePath, "arms"))
			  sprintf(FilePath, "models/player/custom_player/kuristaja/nanosuit/nanosuit_arms.mdl");*/

		/*if (strstr(FilePath, "ctm_"))
			  sprintf(FilePath, "models/player/custom_player/kuristaja/nanosuit/nanosuitv3.mdl");

		  if (strstr(FilePath, "tm_"))
			  sprintf(FilePath, "models/player/custom_player/kuristaja/nanosuit/nanosuitv3.mdl");*/

		return original(ecx, FilePath);
	}

	void __stdcall override_view::hooked(CViewSetup* view)
	{
		no_flash::handle();
		no_smoke::handle();
		clantag::animate();
		color_modulation::handle();

		features::thirdperson();

		if (!g::engine_client->IsConnected() || !g::engine_client->IsInGame())
			return original(g::client_mode, view);

		if (!globals::view_matrix::has_offset)
		{
			globals::view_matrix::has_offset = true;
			globals::view_matrix::offset = (reinterpret_cast<DWORD>(&g::engine_client->WorldToScreenMatrix()) + 0x40);
		}

		if (!g::local_player->m_bIsScoped())
			view->fov = settings::misc::debug_fov;

		original(g::client_mode, view);
	}

	void __stdcall paint_traverse::hooked(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };

		if (g::engine_client->IsInGame() && g::engine_client->IsConnected() && settings::misc::noscope && !strcmp("HudZoom", g::vgui_panel->GetName(panel)))
			return;

		if (settings::misc::smoke_helper)
		{
			visuals::DrawRing3D();
			visuals::DrawRing3DPopflash();
		}

		original(g::vgui_panel, panel, forceRepaint, allowForce);

		if (!panelId)
		{
			const auto panelName = g::vgui_panel->GetName(panel);

			if (!strcmp(panelName, "FocusOverlayPanel"))
				panelId = panel;
		}
		else if (panelId == panel)
		{
			static bool bSkip = false;
			bSkip = !bSkip;

			if (bSkip)
				return;

			if (g::local_player && GetAsyncKeyState(VK_TAB))
				utils::RankRevealAll();
		}
	}

	void __stdcall emit_sound::hooked(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk)
	{
		if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep")) {
			static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(utils::pattern_scan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

			static HWND window = FindWindow(NULL, L"Counter-Strike: Global Offensive");

			if (fnAccept) {
				fnAccept("");

				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = window;
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}

		original(g::engine_sound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);
	}

	void __fastcall scene_end::hooked(IVRenderView*& view)
	{
		visuals::more_chams();

		if (settings::chams::localplayer::desync_chams && settings::desync::enabled)
			visuals::DesyncChams();

		original(view);

		if (!g::engine_client->IsInGame() || !g::engine_client->IsConnected())
			return;
	}

	void __stdcall draw_model_execute::hooked(IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone)
	{
		if (g::mdl_render->IsForcedMaterialOverride() && !strstr(info.pModel->szName, "arms") && !strstr(info.pModel->szName, "weapons/v_"))
			return original(g::mdl_render, context, &state, &info, bone);
		
		Chams::Get().OnDrawModelExecute(context, state, info, bone);
		
		original(g::mdl_render, context, &state, &info, bone);
		
		g::mdl_render->ForcedMaterialOverride(nullptr);
	}

	int __stdcall post_screen_effects::hooked(int value)
	{
		visuals::glow();

		return original(g::client_mode, value);
	}

	bool __stdcall is_connected::hooked()
	{
		static void* force_inventory_open = utils::pattern_scan(("client.dll"), "75 04 B0 01 5F") - 2;

		if (_ReturnAddress() == force_inventory_open && settings::misc::force_inventory_open) {
			return false;
		}

		return original(g::engine_client);
	}
}