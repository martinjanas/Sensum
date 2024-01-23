#include "settings.h"
#include "config.h"
#include "options.hpp"
#include "../jsoncpp/json.h"
#include "../helpers/imdraw.h"
#include "globals.h"

namespace settings
{
	namespace esp
	{
		bool enabled = false;
		bool visible_only = false;
		bool boxes = false;
		int box_type = 0;
		bool names = false;
		bool health = false;
		int health_position = 0;
		bool armour = false;
		int armour_position = 1;
		bool weapons = false;
		int weapon_mode = 0;
		bool ammo = false;
		bool offscreen = false;
		bool is_scoped = false;
		bool dormant = false;
		bool is_flashed = false;
		bool is_defusing = false;
		bool is_reloading = false;
		bool bomb_esp = false;
		bool bone_esp = false;
		bool visible_on_sound = false;
		bool buylog = false;

		Color visible_color = Color(0, 200, 80);
		Color occluded_color = Color(0, 0, 0);
		Color bone_esp_color = Color(255, 255, 255, 255);
	}

	namespace chams
	{
		int chams_list_mode = 0;

		namespace enemy
		{
			bool enabled = false;
			bool visible_only = false;
			bool health_chams = false;

			Color color_visible = Color(0, 128, 0, 255);
			Color color_not_visible = Color(255, 0, 0, 255);
			Color color_backtrack = Color(0, 255, 128, 128);
		}

		namespace teammates
		{
			bool enabled = false;
			bool visible_only = false;
			bool health_chams = false;

			Color color_visible = Color(0, 128, 0, 255);
			Color color_not_visible = Color(255, 0, 0, 255);
			Color color_backtrack = Color(0, 255, 128, 128);
		}
	}

	namespace visuals
	{
		bool grenade_prediction = false;
		bool player_info_box = false;
		bool world_grenades = false;
		bool sniper_crosshair = true;
		bool planted_c4 = false;
		bool dropped_weapons = false;
		bool night_mode = false;
		bool rcs_cross = false;
		float radius = 12;
		int rcs_cross_mode;
		float player_info_box_alpha = 1.0f;
		
		ImVec4 recoil_color = ImVec4{ 0.f, 0.f, 0.f, 1.0f };
		ImVec4 spread_cross_color = ImVec4{ 1.f, 0.f, 0.f, 1.0f };
	}

	namespace aimbot
	{
		int setting_type = settings_type_t::separately;

		std::vector<aimbot_group> m_groups = { };
		std::map<int, weapon_setting> m_items = { };

		bool hello_nigga = true;
	}

	namespace misc
	{
		bool radar = false;
		bool bhop = false;
		bool auto_strafe = false;
		bool selfnade = false;
		float viewmodel_fov = 68.f;
		bool deathmatch = false;
		int debug_fov = 90;
		bool noscope = false;
		bool lefthandknife = false;
		bool fast_stop = false;
		bool smoke_helper = false;
		bool flash_helper = false;
		bool print_fov = false;
	};

	void load(std::string name)
	{
		config::load(name, "configs", true, [](Json::Value root)
			{
				Option::Load(root["esp.enabled"], esp::enabled);//Cant choose team bug
				Option::Load(root["esp.visible_only"], esp::visible_only);
				Option::Load(root["esp.boxes"], esp::boxes, true);
				Option::Load(root["esp.names"], esp::names);
				Option::Load(root["esp.box_type"], esp::box_type, EBoxType::Normal);
				Option::Load(root["esp.health"], esp::health);
				Option::Load(root["esp.health_position"], esp::health_position, 0);
				Option::Load(root["esp.armour"], esp::armour);
				Option::Load(root["esp.armour_position"], esp::armour_position, 1);
				Option::Load(root["esp.weapons"], esp::weapons);
				Option::Load(root["esp.offscreen"], esp::offscreen);
				Option::Load(root["esp.dormant"], esp::dormant);
				Option::Load(root["esp.is_scoped"], esp::is_scoped);
				Option::Load(root["esp.is_flashed"], esp::is_flashed);
				Option::Load(root["esp.is_defusing"], esp::is_defusing);
				Option::Load(root["esp_weapon_mode"], settings::esp::weapon_mode);
				Option::Load(root["esp.player_info_box_alpha"], settings::visuals::player_info_box_alpha);
				Option::Load(root["esp.visible_color"], esp::visible_color, Color(0, 200, 80));
				Option::Load(root["esp.occluded_color"], esp::occluded_color, Color::Black);
		
				Option::Load(root["visuals.world_grenades"], visuals::world_grenades, true);
				Option::Load(root["visuals.sniper_crosshair"], visuals::sniper_crosshair, true);
				Option::Load(root["visuals.grenade_prediction"], visuals::grenade_prediction, true);
				Option::Load(root["visuals.planted_c4"], visuals::planted_c4);
				Option::Load(root["visuals.dropped_weapons"], visuals::dropped_weapons);
				Option::Load(root["visuals.night_mode"], visuals::night_mode);

				Option::Load(root["chams.list_mode"], chams::chams_list_mode, 0);
				Option::Load(root["chams.enemy_enabled"], chams::enemy::enabled, false);
				Option::Load(root["chams.enemy_visible_only"], chams::enemy::visible_only, true);

				Option::Load(root["chams.teammates_enabled"], chams::teammates::enabled, false);
				Option::Load(root["chams.teammates_visible_only"], chams::teammates::visible_only, true);
				
				Option::Load(root["chams.enemy_color_vis"], chams::enemy::color_visible);
				Option::Load(root["chams.team_color_vis"], chams::teammates::color_visible);
				Option::Load(root["chams.enemy_color_xqz"], chams::enemy::color_not_visible);
				Option::Load(root["chams.team_color_xqz"], chams::teammates::color_not_visible);
				
				Option::Load(root["misc.viewmodel_fov"], misc::viewmodel_fov, 68);
				Option::Load(root["misc.deathmatch"], misc::deathmatch);
				Option::Load(root["misc.radar"], misc::radar);
				Option::Load(root["misc.debug_fov"], misc::debug_fov);
				Option::Load(root["misc.rcs_cross"], visuals::rcs_cross);
				Option::Load(root["misc.rcs_cross_mode"], visuals::rcs_cross_mode);
				Option::Load(root["misc.radius"], visuals::radius, 12.f);
				Option::Load(root["mics.rcs_cross_color"], visuals::recoil_color);
				Option::Load(root["misc.no_scope_overlay"], misc::noscope);
				Option::Load(root["misc.bomb_damage_esp"], esp::bomb_esp);
				Option::Load(root["misc_spread_crosshair_color"], settings::visuals::spread_cross_color);
				Option::Load(root["misc_esp_ammo"], settings::esp::ammo);
				Option::Load(root["misc.player_info_box"], settings::visuals::player_info_box);
				Option::Load(root["misc.left_hand_knife"], settings::misc::lefthandknife);
				Option::Load(root["misc.fast_stop"], settings::misc::fast_stop);
				Option::Load(root["misc.smoke_helper"], misc::smoke_helper);
				Option::Load(root["misc.flash_helper"], misc::flash_helper);
				Option::Load(root["misc.print_fov"], misc::print_fov);
				Option::Load(root["aimbot.nigga_backtrack_off"], aimbot::hello_nigga);

				Option::Load(root["binds.esp"], globals::binds::esp, 0);
				Option::Load(root["binds.trigger"], globals::binds::trigger, 18);
				Option::Load(root["binds.thirdperson"], globals::binds::thirdperson::key, 86);

				Json::Value config_binds = root["binds.configs"];
				if (!config_binds.empty())
				{
					for (Json::Value::iterator it = config_binds.begin(); it != config_binds.end(); ++it)
					{
						Json::Value settings = *it;

						if (settings["key"].asInt() > 0)
							globals::binds::configs[settings["name"].asString()] = settings["key"].asInt();
					}
				}

				Option::Load(root["legitbot.setting_type"], aimbot::setting_type);

				aimbot::m_items.clear();

				Json::Value aimbot_items = root["legitbot.items"];
				for (Json::Value::iterator it = aimbot_items.begin(); it != aimbot_items.end(); ++it)
				{
					Json::Value settings = *it;

					weapon_setting data;
					Option::Load(settings["enabled"], data.enabled);
					Option::Load(settings["back_shot"], data.back_shot);
					Option::Load(settings["autodelay"], data.autodelay);
					Option::Load(settings["autopistol"], data.autopistol);
					Option::Load(settings["check_smoke"], data.check_smoke);
					Option::Load(settings["check_flash"], data.check_flash);
					Option::Load(settings["check_air"], data.check_air, true);
					Option::Load(settings["backtrack.enabled"], data.backtrack.enabled);
					Option::Load(settings["backtrack.legit"], data.backtrack.legit);
					Option::Load(settings["autostop"], data.auto_stop);
					Option::Load(settings["dynamic_fov"], data.dynamic_fov);
					Option::Load(settings["backtrack.time"], data.backtrack.ticks, 6);
					if (data.backtrack.ticks < 0 || data.backtrack.ticks > 12)
						data.backtrack.ticks = 6;

					Option::Load(settings["check_zoom"], data.check_zoom, true);
					
					Option::Load(settings["trigger.enabled"], data.trigger.enabled);
					Option::Load(settings["trigger.delay"], data.trigger.delay);
					Option::Load(settings["trigger.delay_btw_shots"], data.trigger.delay_btw_shots, 50);
					Option::Load(settings["trigger.hitchance"], data.trigger.hitchance, 1);
					Option::Load(settings["trigger.magnet"], data.trigger.magnet);

					Option::Load(settings["hitboxes.head"], data.hitboxes.head, true);
					Option::Load(settings["hitboxes.neck"], data.hitboxes.neck, true);
					Option::Load(settings["hitboxes.body"], data.hitboxes.body, true);
					Option::Load(settings["hitboxes.hands"], data.hitboxes.arms);
					Option::Load(settings["hitboxes.legs"], data.hitboxes.legs);

					Option::Load(settings["fov"], data.fov);
					Option::Load(settings["smooth"], data.smooth, 1);
					Option::Load(settings["shot_delay"], data.shot_delay);
					Option::Load(settings["kill_delay"], data.kill_delay, 600);
					Option::Load(settings["by_damage"], data.by_damage);
					Option::Load(settings["min_damage"], data.min_damage, 0);
					Option::Load(settings["hitchanse"], data.min_hitchance, 0);
					Option::Load(settings["rcs_hitbox_override"], data.rcs_override_hitbox);
					Option::Load(settings["extended_fov"], data.extended_fov);
					Option::Load(settings["extended_smooth"], data.extended_smooth);
					Option::Load(settings["fov_smooth_override"], data.fov_smooth_override);
					Option::Load(settings["min_shots_fired"], data.min_shots_fired, 3);

					Option::Load(settings["autowall.enabled"], data.autowall.enabled);
					Option::Load(settings["autowall.min_damage"], data.autowall.min_damage, 1);

					Option::Load(settings["rcs"], data.recoil.enabled);
					Option::Load(settings["rcs.first_bullet"], data.recoil.first_bullet);
					Option::Load(settings["rcs.humanized"], data.recoil.humanized);
					Option::Load(settings["rcs_value_x"], data.recoil.pitch, 100);
					Option::Load(settings["rcs_value_y"], data.recoil.yaw, 100);

					aimbot::m_items[it.key().asInt()] = data;
				}

				settings::aimbot::m_groups.clear();

				for (auto& group : root["aimbot.groups"])
				{
					std::vector<int> weapons = { };

					for (auto& weapon_index : group["weapons"])
					{
						weapons.emplace_back(weapon_index.asInt());
					}

					settings::aimbot::m_groups.emplace_back(aimbot_group{ group["name"].asString(), weapons });
				}
			});
	}

	void save(std::string name)
	{
		config::save(name, "configs", true, []()
			{
				Json::Value config;

				config["esp.enabled"] = esp::enabled;
				config["esp.visible_only"] = esp::visible_only;
				config["esp.boxes"] = esp::boxes;
				config["esp.names"] = esp::names;
				config["esp.box_type"] = esp::box_type;
				config["esp.health"] = esp::health;
				config["esp.health_position"] = esp::health_position;
				config["esp.armour"] = esp::armour;
				config["esp.armour_position"] = esp::armour_position;
				config["esp.weapons"] = esp::weapons;
				config["esp.offscreen"] = esp::offscreen;
				config["esp.dormant"] = esp::dormant;
				config["esp.is_scoped"] = esp::is_scoped;
				config["esp.is_defusing"] = esp::is_defusing;
				config["esp.is_flashed"] = esp::is_flashed;
				config["esp_weapon_mode"] = settings::esp::weapon_mode;
				config["esp.player_info_box_alpha"] = settings::visuals::player_info_box_alpha;
				
				Option::Save(config["esp.visible_color"], esp::visible_color);
				Option::Save(config["esp.occluded_color"], esp::occluded_color);
				
				config["visuals.world_grenades"] = visuals::world_grenades;
				config["visuals.sniper_crosshair"] = visuals::sniper_crosshair;
				config["visuals.grenade_prediction"] = visuals::grenade_prediction;
				config["visuals.planted_c4"] = visuals::planted_c4;
				config["visuals.dropped_weapons"] = visuals::dropped_weapons;
				config["visuals.night_mode"] = visuals::night_mode;

				config["chams.list_mode"] = chams::chams_list_mode;
				config["chams.enemy_enabled"] = chams::enemy::enabled;
				config["chams.enemy_visible_only"] = chams::enemy::visible_only;

				config["chams.teammates_enabled"] = chams::teammates::enabled;
				config["chams.teammates_visible_only"] = chams::teammates::visible_only;
	
				Option::Save(config["chams.enemy_color_vis"], chams::enemy::color_visible);
				Option::Save(config["chams.team_color_vis"], chams::teammates::color_visible);
				Option::Save(config["chams.enemy_color_xqz"], chams::enemy::color_not_visible);
				Option::Save(config["chams.team_color_xqz"], chams::teammates::color_not_visible);
				
				config["misc.viewmodel_fov"] = misc::viewmodel_fov;
				config["misc.deathmatch"] = misc::deathmatch;
				config["misc.radar"] = misc::radar;
				config["misc.debug_fov"] = misc::debug_fov;
				config["misc.rcs_cross"] = visuals::rcs_cross;
				config["misc.rcs_cross_mode"] = visuals::rcs_cross_mode;
				config["misc.radius"] = visuals::radius;
				config["misc.no_scope_overlay"] = misc::noscope;
				config["misc.bomb_damage_esp"] = esp::bomb_esp;
				config["misc_esp_ammo"] = settings::esp::ammo;
				config["misc.player_info_box"] = settings::visuals::player_info_box;
				config["misc.left_hand_knife"] = settings::misc::lefthandknife;
				config["misc.fast_stop"] = settings::misc::fast_stop;
				config["misc.smoke_helper"] = misc::smoke_helper;
				config["misc.flash_helper"] = misc::flash_helper;
				config["misc.print_fov"] = misc::print_fov;
				config["aimbot.nigga_backtrack_off"] = aimbot::hello_nigga;
				
				config["binds.esp"] = globals::binds::esp;
				config["binds.trigger"] = globals::binds::trigger;
				config["binds.thirdperson"] = globals::binds::thirdperson::key;

				Json::Value config_binds;
				for (auto& bind : globals::binds::configs)
				{
					Json::Value bind_data;
					bind_data["key"] = bind.second;
					bind_data["name"] = bind.first;

					config_binds.append(bind_data);
				}

				config["binds.configs"] = config_binds;

				Option::Save(config["misc_spread_crosshair_color"], settings::visuals::spread_cross_color);
				Option::Save(config["mics.rcs_cross_color"], visuals::recoil_color);

				config["legitbot.setting_type"] = aimbot::setting_type;

				Json::Value aimbot_items;
				for (const auto& data : aimbot::m_items)
				{
					Json::Value act;
					const auto aimbot_data = data.second;
					act["back_shot"] = aimbot_data.back_shot;
					act["autodelay"] = aimbot_data.autodelay;
					act["autopistol"] = aimbot_data.autopistol;
					act["autowall.enabled"] = aimbot_data.autowall.enabled;
					act["autowall.min_damage"] = aimbot_data.autowall.min_damage;
					act["backtrack.enabled"] = aimbot_data.backtrack.enabled;
					act["backtrack.legit"] = aimbot_data.backtrack.legit;
					act["backtrack.ticks"] = aimbot_data.backtrack.ticks;
					act["check_flash"] = aimbot_data.check_flash;
					act["check_smoke"] = aimbot_data.check_smoke;
					act["check_air"] = aimbot_data.check_air;
					act["enabled"] = aimbot_data.enabled;
					act["fov"] = aimbot_data.fov;
					act["extended_fov"] = aimbot_data.extended_fov;
					act["extended_smooth"] = aimbot_data.extended_smooth;
					act["fov_smooth_override"] = aimbot_data.fov_smooth_override;
					act["min_shots_fired"] = aimbot_data.min_shots_fired;
					act["by_damage"] = aimbot_data.by_damage;
					act["min_damage"] = aimbot_data.min_damage;
					act["hitchanse"] = aimbot_data.min_hitchance;
					act["rcs_hitbox_override"] = aimbot_data.rcs_override_hitbox;
					act["autostop"] = aimbot_data.auto_stop;
					act["dynamic_fov"] = aimbot_data.dynamic_fov;
					
					act["trigger.enabled"] = aimbot_data.trigger.enabled;
					act["trigger.delay"] = aimbot_data.trigger.delay;
					act["trigger.delay_btw_shots"] = aimbot_data.trigger.delay_btw_shots;
					act["trigger.hitchance"] = aimbot_data.trigger.hitchance;
					act["trigger.magnet"] = aimbot_data.trigger.magnet;

					act["hitboxes.head"] = aimbot_data.hitboxes.head;
					act["hitboxes.neck"] = aimbot_data.hitboxes.neck;
					act["hitboxes.body"] = aimbot_data.hitboxes.body;
					act["hitboxes.hands"] = aimbot_data.hitboxes.arms;
					act["hitboxes.legs"] = aimbot_data.hitboxes.legs;

					act["kill_delay"] = aimbot_data.kill_delay;
					act["check_zoom"] = aimbot_data.check_zoom;
					act["shot_delay"] = aimbot_data.shot_delay;
					act["smooth"] = aimbot_data.smooth;
					act["min_damage"] = aimbot_data.min_damage;

					act["rcs"] = aimbot_data.recoil.enabled;
					act["rcs.first_bullet"] = aimbot_data.recoil.first_bullet;
					act["rcs.humanized"] = aimbot_data.recoil.humanized;
					act["rcs_value_x"] = aimbot_data.recoil.pitch;
					act["rcs_value_y"] = aimbot_data.recoil.yaw;

					aimbot_items[data.first] = act;
				}

				config["legitbot.items"] = aimbot_items;

				Json::Value aimbot_groups;
				for (auto& group : aimbot::m_groups)
				{
					Json::Value act;
					act["name"] = group.name;

					Json::Value weapons;
					for (auto& weapon : group.weapons)
						weapons.append(weapon);

					act["weapons"] = weapons;

					aimbot_groups.append(act);
				}

				config["aimbot.groups"] = aimbot_groups;

				return config;
			});
	}
}