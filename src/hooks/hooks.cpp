#pragma warning(disable : 26812)

#include <format>

#include "hooks.h"
#include "../settings/globals.h"
#include "../helpers/console.h"
#include "../hooks/hooked_functions/game_event_listener.h"

namespace hooks
{	
	CGameEventListener event_listener = CGameEventListener();

	bool init()
	{
		sequence::hook = new recv_prop_hook(c_base_view_model::m_nSequence(), sequence::hooked);
		end_scene::setup = reinterpret_cast<void*>(utils::get_virtual(g::d3_device, end_scene::index));
		create_move::setup = reinterpret_cast<void*>(utils::get_virtual(g::client_mode, create_move::index));
		reset::setup = reinterpret_cast<void*>(utils::get_virtual(g::d3_device, reset::index));
		paint_traverse::setup = reinterpret_cast<void*>(utils::get_virtual(g::vgui_panel, paint_traverse::index));
		override_view::setup = reinterpret_cast<void*>(utils::get_virtual(g::client_mode, override_view::index));
		frame_stage_notify::setup = reinterpret_cast<void*>(utils::get_virtual(g::base_client, frame_stage_notify::index));
		draw_model_execute::setup = reinterpret_cast<void*>(utils::get_virtual(g::mdl_render, draw_model_execute::index));

		//One of those causes crashing: OUTDATED!

		//get_color_modulation::setup = reinterpret_cast<void*>(utils::pattern_scan("materialsystem.dll", "55 8B EC 83 EC ? 56 8B F1 8A 46"));
		//is_using_static_prop_debug_modes::setup = reinterpret_cast<void*>(utils::pattern_scan("engine.dll", "8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D"));
		
		//if (MH_CreateHook(get_color_modulation::setup, &get_color_modulation::hooked, reinterpret_cast<void**>(&get_color_modulation::original)) != MH_OK)
			//return false;

		//if (MH_CreateHook(is_using_static_prop_debug_modes::setup, &is_using_static_prop_debug_modes::hooked, reinterpret_cast<void**>(&is_using_static_prop_debug_modes::original)) != MH_OK)
			//return false;
		
		if (MH_CreateHook(end_scene::setup, &end_scene::hooked, reinterpret_cast<void**>(&end_scene::original)) != MH_OK)
			return false;
			
		if (MH_CreateHook(create_move::setup, &create_move::hooked, reinterpret_cast<void**>(&create_move::original)) != MH_OK)
			return false;

		if (MH_CreateHook(reset::setup, &reset::hooked, reinterpret_cast<void**>(&reset::original)) != MH_OK)
			return false;
		
		if (MH_CreateHook(paint_traverse::setup, &paint_traverse::hooked, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK)
			return false;
		
		if (MH_CreateHook(override_view::setup, &override_view::hooked, reinterpret_cast<void**>(&override_view::original)) != MH_OK)
			return false;
			
		if (MH_CreateHook(frame_stage_notify::setup, &frame_stage_notify::hooked, reinterpret_cast<void**>(&frame_stage_notify::original)) != MH_OK)
			return false;
		
		if (MH_CreateHook(draw_model_execute::setup, &draw_model_execute::hooked, reinterpret_cast<void**>(&draw_model_execute::original)) != MH_OK)
			return false;

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			return false;

		g::game_events->add_listener(&event_listener, xorstr_("game_newmap"), false);
		g::game_events->add_listener(&event_listener, xorstr_("player_hurt"), false);
		g::game_events->add_listener(&event_listener, xorstr_("bomb_begindefuse"), false);
		g::game_events->add_listener(&event_listener, xorstr_("item_purchase"), false);
		
		
		return true;
	}

	void destroy()
	{
		//static auto weapon_debug_spread_show = g::cvar->find(xorstr_("weapon_debug_spread_show"));
		//weapon_debug_spread_show->SetValue(0);

		//static auto engine_no_focus_sleep = g::cvar->find(xorstr_("engine_no_focus_sleep"));
		//engine_no_focus_sleep->SetValue(50);

		g::game_events->remove_listener(&event_listener);
		
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);

		MH_Uninitialize();

		sequence::hook->~recv_prop_hook();
	}
}