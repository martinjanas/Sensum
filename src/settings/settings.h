#pragma once

#include "../imgui/imgui.h"
#include "../valve_sdk/csgostructs.hpp"

struct aimbot_group
{
	std::string name;
	std::vector<int> weapons = { };
};

enum settings_type_t : int
{
	separately = 0,
	subgroups,
	for_all,
	groups,
};

#pragma pack(push, 1)
struct weapon_setting
{
	bool enabled = false;
	bool autopistol = false;
	bool auto_stop = false;
	bool check_smoke = false;
	bool check_flash = false;
	bool check_zoom = true;
	bool check_air = true;
	bool by_damage = false;
	bool back_shot = false;
	bool rcs_override_hitbox = false;
	bool dynamic_fov = true;

	int min_damage = 0;
	int min_hitchance = 0;

	int rcs_type = 0;

	struct
	{
		bool enabled = false;
		int ticks = 6;
		bool legit = false;
	} backtrack;

	struct
	{
		bool auto_delay = false;

		int before_shot = 0;
		int target_switch = 600;
	} delays;

	bool autodelay = false;

	struct
	{
		bool enabled = false;
		bool magnet = true;
		int delay = 0;
		int delay_btw_shots = 50;
		int hitchance = 1;
	} trigger;

	struct {
		bool enabled = false;
		bool first_bullet = false;
		bool humanized = false;
		
		int pitch = 100;
		int yaw = 100;
	} recoil;

	struct
	{
		bool head = true;
		bool neck = true;
		bool body = true;
		bool arms = true;
		bool legs = true;
	} hitboxes;

	struct
	{
		bool enabled = false;
		int min_damage = 1;
	} autowall;

	float fov = 0.f;
	float smooth = 1.f;
	int shot_delay = 0;
	int kill_delay = 600;

	float extended_fov = 0.f;
	float extended_smooth = 1.f;
	bool fov_smooth_override = false;
	int min_shots_fired = 3;
};
#pragma pack(pop)

namespace settings
{
	void load(std::string name);
	void save(std::string name);

	namespace esp
	{
		extern bool enabled;
		extern bool visible_only;
		extern bool boxes;
		extern int box_type;
		extern bool names;
		extern bool health;
		extern int health_position;
		extern bool armour;
		extern int armour_position;
		extern bool weapons;
		extern int weapon_mode;
		extern bool ammo;
		extern bool offscreen;
		extern bool is_scoped;
		extern bool is_flashed;
		extern bool is_defusing;
		extern bool dormant;
		extern bool is_reloading;
		extern bool bomb_esp;
		extern bool ammo;
		extern bool bone_esp;
		extern bool visible_on_sound;
		extern bool buylog;

		extern Color visible_color;
		extern Color occluded_color;
		extern Color bone_esp_color;
	}

	namespace chams
	{
		extern int chams_list_mode;

		namespace enemy
		{
			extern bool enabled;
			extern bool visible_only;
			extern bool health_chams;
			
			extern Color color_visible;
			extern Color color_not_visible;
			extern Color color_backtrack;
		}

		namespace teammates
		{
			extern bool enabled;
			extern bool visible_only;
			extern bool health_chams;
			
			extern Color color_visible;
			extern Color color_not_visible;
			extern Color color_backtrack;
		}
	}
	
	namespace visuals
	{
		extern bool grenade_prediction;
		extern bool player_info_box;
		extern bool world_grenades;
		extern bool sniper_crosshair;
		extern bool planted_c4;
		extern bool dropped_weapons;
		extern bool night_mode;
		extern bool rcs_cross;
		extern float radius;
		extern int rcs_cross_mode;
		extern float player_info_box_alpha;

		extern ImVec4 recoil_color;
	}

	namespace aimbot
	{
		extern int setting_type;

		extern std::vector<aimbot_group> m_groups;
		extern std::map<int, weapon_setting> m_items;

		extern bool hello_nigga;
	}

	namespace misc
	{
		extern float viewmodel_fov;
		extern bool deathmatch;
		extern bool radar;
		extern bool bhop;
		extern bool auto_strafe;
		extern bool selfnade;
		extern int debug_fov;
		extern bool noscope;
		extern bool lefthandknife;
		extern bool fast_stop;
		extern bool smoke_helper;
		extern bool flash_helper;
		extern bool print_fov;
	}
}
