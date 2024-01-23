#include "features.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/console.h"
#include "../helpers/notifies.h"

#include <string>

namespace color_modulation
{
	auto post_processing = false;
	auto night_mode_state = false;
	
	auto debug_crosshair = -1.f;
	auto view_model_fov = -1.f;
	auto debug_model_fov = -1.f;

	ConVar* viewmodel_fov = nullptr;
	ConVar* mat_postprocess_enable = nullptr;
	
	void night_mode()
	{
		reinterpret_cast<void(__fastcall*)(const char*)>(patterns::set_sky_addr)(xorstr_("sky_csgo_night02"));
	}

	void event()
	{
		if (night_mode_state && !settings::visuals::night_mode)
			reinterpret_cast<void(__fastcall*)(const char*)>(patterns::set_sky_addr)(xorstr_("vertigoblue_hdr"));

		night_mode_state = !night_mode_state;
		debug_crosshair = -1;
	}

	bool is_vars_changed()
	{
		if (night_mode_state != settings::visuals::night_mode)
			return true;

		if (post_processing != globals::post_processing)
			return true;

		if (view_model_fov != settings::misc::viewmodel_fov)
			return true;

		return false;
	}

	void set_convars()
	{
		if (!viewmodel_fov)
		{
			viewmodel_fov = g::cvar->find(xorstr_("viewmodel_fov"));
			viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!mat_postprocess_enable)
		{
			mat_postprocess_enable = g::cvar->find(xorstr_("mat_postprocess_enable"));
			mat_postprocess_enable->m_fnChangeCallbacks.m_Size = 0;
		}

		g::cvar->find(xorstr_("engine_no_focus_sleep"))->SetValue(0);
		g::cvar->find(xorstr_("violence_ablood"))->SetValue(0);
		g::cvar->find(xorstr_("violence_hblood"))->SetValue(0);

		viewmodel_fov->SetValue(settings::misc::viewmodel_fov);
		mat_postprocess_enable->SetValue(post_processing ? 1 : 0);
	}

	void sniper_crosshair()
	{
		if (!g::local_player)
			return;

		bool is_scoped = g::local_player->m_bIsScoped();
		if (!g::local_player->IsAlive() && g::local_player->m_hObserverTarget())
		{
			auto observer = (c_base_player*)c_base_player::GetEntityFromHandle(g::local_player->m_hObserverTarget());
			if (observer && observer->IsPlayer())
				is_scoped = observer->m_bIsScoped();
		}

		static auto weapon_debug_spread_show = g::cvar->find(xorstr_("weapon_debug_spread_show"));

		if (settings::visuals::sniper_crosshair)
		{
			if (debug_crosshair != 0 && is_scoped)
			{
				debug_crosshair = 0;
				weapon_debug_spread_show->SetValue(0);
			}

			if (debug_crosshair != 3 && !is_scoped)
			{
				debug_crosshair = 3;
				weapon_debug_spread_show->SetValue(3);
			}
		}
		else if (debug_crosshair != 0)
		{
			debug_crosshair = 0;
			weapon_debug_spread_show->SetValue(0);
		}
	}

	void handle()
	{
		sniper_crosshair();

		if (!is_vars_changed())
			return;

		post_processing = globals::post_processing;
		view_model_fov = settings::misc::viewmodel_fov;
		night_mode_state = settings::visuals::night_mode;
			
		set_convars();
		
		if (settings::visuals::night_mode)
			night_mode();
	}
}