#include "features.h"
#include "../settings/globals.h"
#include "../settings/options.hpp"
#include "../helpers/console.h"

#include <string>

namespace color_modulation
{
	auto debug_crosshair = -1;
	auto view_model_fov = -1.f;
	auto debug_model_fov = -1;

	auto post_processing = false;
	auto night_mode_state = false;
	auto night_mode_first = false;

	auto flViewmodel_offset_x = -1.f;
	auto flViewmodel_offset_y = -1.f;
	auto flViewmodel_offset_z = -1.f;

	ConVar* viewmodel_fov = nullptr;
	ConVar* debug_fov = nullptr;
	ConVar* r_3dsky = nullptr;
	ConVar* r_drawspecificstaticprop = nullptr;
	ConVar* engine_focus = nullptr;
	ConVar* cl_threaded_bone_setup = nullptr;
	ConVar* r_modelAmbientMin = nullptr;
	ConVar* mat_postprocess_enable = nullptr;
	ConVar* mat_force_tonemap_scale = nullptr;
	ConVar* viewmodel_offset_x = nullptr;
	ConVar* viewmodel_offset_y = nullptr;
	ConVar* viewmodel_offset_z = nullptr;

	const uint32_t model_textures = FNV("Model textures");
	const uint32_t skybox_textures = FNV("SkyBox textures");
	const uint32_t world_textures = FNV("World textures");
	const uint32_t static_prop_textures = FNV("StaticProp textures");
	const uint32_t particle_textures = FNV("Particle textures");

	static const char* sv_skyname_backup;

	uint8_t* sky_fn_offset = nullptr;

	const char* GetMapOriginalSkybox()
	{
		return g::cvar->find(xorstr_("sv_skyname"))->GetString();
	}

	void NightMode()
	{
		if (!r_3dsky)
		{
			r_3dsky = g::cvar->find("r_3dsky");
			r_3dsky->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!r_drawspecificstaticprop)
		{
			r_drawspecificstaticprop = g::cvar->find("r_drawspecificstaticprop");
			r_drawspecificstaticprop->m_fnChangeCallbacks.m_Size = 0;
		}

		static auto sv_skyname = g::cvar->find("sv_skyname");
		sv_skyname->SetValue("sky_csgo_night02");
		
		r_3dsky->SetValue(0);
		r_drawspecificstaticprop->SetValue(settings::visuals::night_mode ? 0 : 1);
	}

	void event()
	{
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

		if (debug_model_fov != settings::misc::debug_fov)
			return true;

		if (flViewmodel_offset_x != settings::misc::viewmodel_offset_x)
			return true;

		if (flViewmodel_offset_y != settings::misc::viewmodel_offset_y)
			return true;

		if (flViewmodel_offset_z != settings::misc::viewmodel_offset_z)
			return true;

		return false;
	}

	void sky_changer()
	{
		static auto sv_skyname = g::cvar->find(xorstr_("sv_skyname"));

		if (!settings::visuals::night_mode)
		{
			switch (settings::visuals::skychanger_mode)
			{
			case 1: //Baggage
				sv_skyname->SetValue("cs_baggage_skybox");
				break;
			case 2: //Tibet
				sv_skyname->SetValue("cs_tibet");
				break;
			case 3: //Embassy
				sv_skyname->SetValue("embassy");
				break;
			case 4: //Italy
				sv_skyname->SetValue("italy");
				break;
			case 5: //Daylight 1
				sv_skyname->SetValue("sky_cs15_daylight01_hdr");
				break;
			case 6: //Daylight 2
				sv_skyname->SetValue("sky_cs15_daylight02_hdr");
				break;
			case 7: //Daylight 3
				sv_skyname->SetValue("sky_cs15_daylight03_hdr");
				break;
			case 8: //Daylight 4
				sv_skyname->SetValue("sky_cs15_daylight04_hdr");
				break;
			case 9: //Cloudy
				sv_skyname->SetValue("sky_csgo_cloudy01");
				break;
			case 10: //Night 1
				sv_skyname->SetValue("sky_csgo_night02");
				break;
			case 11: //Night 2
				sv_skyname->SetValue("sky_csgo_night02b");
				break;
			case 12: //Night Flat
				sv_skyname->SetValue("sky_csgo_night_flat");
				break;
			case 13: //Day HD
				sv_skyname->SetValue("sky_day02_05_hdr");
				break;
			case 14: //Day
				sv_skyname->SetValue("sky_day02_05");
				break;
			case 15: //Rural
				sv_skyname->SetValue("sky_l4d_rural02_ldr");
				break;
			case 16: //Vertigo HD
				sv_skyname->SetValue("vertigo_hdr");
				break;
			case 17: //Vertigo Blue HD
				sv_skyname->SetValue("vertigoblue_hdr");
				break;
			case 18: //Vertigo
				sv_skyname->SetValue("vertigo");
				break;
			case 19: //Vietnam
				sv_skyname->SetValue("vietnam");
				break;
			case 20: //Dusty Sky
				sv_skyname->SetValue("sky_dust");
				break;
			case 21: //Jungle
				sv_skyname->SetValue("jungle");
				break;
			case 22: //Nuke
				sv_skyname->SetValue("nukeblank");
				break;
			case 23: //Office
				sv_skyname->SetValue("office");
				break;
			default:
				break;
			}
		}
	}

	void music_kit_changer()
	{
		if (settings::misc::music_kit_apply)
		{
			auto player_resource = *g::player_resource;

			if (player_resource)
			{
				player_resource->GetMusicKit()[g::local_player->EntIndex()] = settings::misc::music_kit + 1;
			}
		}
	}

	void set_convars()
	{
		if (!viewmodel_fov)
		{
			viewmodel_fov = g::cvar->find(xorstr_("viewmodel_fov"));
			viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!debug_fov)
		{
			debug_fov = g::cvar->find(xorstr_("fov_cs_debug"));
			debug_fov->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!r_modelAmbientMin)
		{
			r_modelAmbientMin = g::cvar->find(xorstr_("r_modelAmbientMin"));
			r_modelAmbientMin->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!mat_postprocess_enable)
		{
			mat_postprocess_enable = g::cvar->find(xorstr_("mat_postprocess_enable"));
			mat_postprocess_enable->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!mat_force_tonemap_scale)
		{
			mat_force_tonemap_scale = g::cvar->find(xorstr_("mat_force_tonemap_scale"));
			mat_force_tonemap_scale->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!viewmodel_offset_x)
		{
			viewmodel_offset_x = g::cvar->find(xorstr_("viewmodel_offset_x"));
			viewmodel_offset_x->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!viewmodel_offset_y)
		{
			viewmodel_offset_y = g::cvar->find(xorstr_("viewmodel_offset_y"));
			viewmodel_offset_y->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!viewmodel_offset_z)
		{
			viewmodel_offset_z = g::cvar->find(xorstr_("viewmodel_offset_z"));
			viewmodel_offset_z->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!engine_focus)
		{
			engine_focus = g::cvar->find(xorstr_("engine_no_focus_sleep"));
			engine_focus->m_fnChangeCallbacks.m_Size = 0;
		}

		if (!cl_threaded_bone_setup)
		{
			cl_threaded_bone_setup = g::cvar->find(xorstr_("cl_threaded_bone_setup"));
			cl_threaded_bone_setup->m_fnChangeCallbacks.m_Size = 0;
		}

		r_modelAmbientMin->SetValue(settings::visuals::night_mode ? 1.f : 0.f);
		
		if(mat_force_tonemap_scale)
		   mat_force_tonemap_scale->SetValue(settings::visuals::night_mode ? 0.2f : 1.f);

		viewmodel_fov->SetValue(settings::misc::viewmodel_fov);
		mat_postprocess_enable->SetValue(post_processing ? 1 : 0);
		engine_focus->SetValue(0);
		cl_threaded_bone_setup->SetValue(1); //Threaded bone setup for more fps, found this while typing "find bones" into the console.
		//If you google its cvar name there should be reddit post about it and how it aparently boosts fps, the post is from 2015.

		static auto backup_viewmodel_x = g::cvar->find("viewmodel_offset_x")->GetFloat();
		static auto backup_viewmodel_y = g::cvar->find("viewmodel_offset_y")->GetFloat();
		static auto backup_viewmodel_z = g::cvar->find("viewmodel_offset_z")->GetFloat();

		if (settings::misc::override_viewmodel)
		{
			viewmodel_offset_x->SetValue(settings::misc::viewmodel_offset_x);
			viewmodel_offset_y->SetValue(settings::misc::viewmodel_offset_y);
			viewmodel_offset_z->SetValue(settings::misc::viewmodel_offset_z);
		}
		else
		{
			viewmodel_offset_x->SetValue(backup_viewmodel_x);
			viewmodel_offset_y->SetValue(backup_viewmodel_y);
			viewmodel_offset_z->SetValue(backup_viewmodel_z);
		}
	}

	void set_material_tone()
	{
		if (!settings::visuals::night_mode)
			return;

		static auto mat_force_tonemap_scale = g::cvar->find(xorstr_("mat_force_tonemap_scale"));

		mat_force_tonemap_scale->SetValue(settings::esp::mfts);
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
		debug_model_fov = settings::misc::debug_fov;
	
		set_material_tone();
		set_convars();
		
		if (settings::visuals::night_mode)
			NightMode();
	}
}