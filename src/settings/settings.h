#pragma once

#include "../imgui/imgui.h"
#include "../valve_sdk/csgostructs.hpp"

struct aimbot_group
{
	std::string name;
	std::vector<int> weapons = { };
};

struct k_weapon_data {
	int itemDefinitionIndex;
	int paintKit;
	int rarity;
	int seed;
	float wear;
	char* name = "";
};

enum settings_type_t : int
{
	separately = 0,
	subgroups,
	for_all,
	groups,
};

enum trigger_type
{
	in_fov = 0,
	in_crosshair,
};

enum fov_type_t : int
{
	statical = 0,
	dynamic,
	rage,
};

#pragma pack(push, 1)
struct weapon_setting
{
	bool enabled = false;
	bool autopistol = false;
	bool check_smoke = false;
	bool check_flash = false;
	bool check_zoom = true;
	bool check_air = true;
	bool by_damage = false;
	bool back_shot = false;
	bool rcs_override_hitbox = false;

	int min_damage = 0;
	int min_hitchanse = 0;

	int rcs_type = 0;

	struct
	{
		bool enabled = false;
		bool bt2 = false;
		int ticks = 6;
		bool legit = false;
		float time = 0.2f;
	} backtrack;

	bool autodelay = false;

	struct
	{
		bool enabled = false;
		bool in_radius = false;
		int delay = 0;
		int delay_btw_shots = 50;
		int hitchance = 1;
	} trigger;

	struct
	{
		bool enabled = false;
		bool always = false;
		float fov = 0.f;
		int chanse = 7;
		bool with_smooth = true;
	} silent;

	struct {
		bool enabled = false;
		bool first_bullet = false;
		bool humanize = false;
		bool standalone = true;
		bool rcs2 = false;

		float pitch = 2.0f;
		float yaw = 2.0f;
	} recoil;

	struct
	{
		bool head = true;
		bool neck = true;
		bool body = true;
		bool hands = true;
		bool legs = true;
	} hitboxes;

	struct
	{
		bool enabled = false;
		int min_damage = 1;
	} autowall;

	bool dynamic_fov = false;
	float fov = 0.f;
	float smooth = 1;
	int shot_delay = 0;
	int kill_delay = 600;
};
#pragma pack(pop)

enum fake_lag_types : int
{
	lag_always = 0,
	lag_only_in_air,
	lag_when_pick,
	lag_by_button,
	lag_adaptive,
};

namespace settings
{
	void load(std::string name);
	void save(std::string name);

	namespace movement
	{
		extern bool engine_prediction;
	}

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
		extern bool sound;
		extern bool snaplines;
		extern bool is_scoped;
		extern bool is_flashed;
		extern bool is_defusing;
		extern bool dormant;
		extern bool beams;
		extern int esp_on_chance;
		extern int esp_off_chance;
		extern bool is_reloading;
		extern float mfts;
		extern bool mat_force;
		extern bool drawFov;
		extern bool bomb_esp;
		extern bool kevlarinfo;
		extern bool haskit;
		extern bool money;
		extern bool ammo;
		extern bool antiobs;
		extern bool soundesp;
		extern bool mat_force_apply;
		extern bool is_desyncing;
		extern bool buylog;
		extern bool bone_esp;

		extern Color visibleColor;
		extern Color occludedColor;
		extern Color colorSoundEsp;
		extern Color aa_indicator_color;
		extern Color bone_esp_color;
	}

	namespace glow
	{
		extern bool glowEnemyEnabled;
		extern bool glowTeamEnabled;
		extern bool glowC4PlantedEnabled;
		extern bool glowDroppedWeaponsEnabled;
		extern bool glowDroppedKitsEnabled;
		extern bool glowNadesEnabled;
		extern bool glowOverride;
		
		extern int style_enemy;
		extern int style_teammate;
		extern int glow_list_mode;

		extern Color glowEnemyColor;
		extern Color glowTeamColor;
		extern Color glowC4PlantedColor;
		extern Color glowDroppedWeaponsColor;
		extern Color glowDroppedKitsColor;
		extern Color glowNadesColor;
		extern Color glowHE;
		extern Color glowMolotovIncendiary;
		extern Color glowFlashbang;
		extern Color glowDecoy;
		extern Color glowSmoke;
		extern Color glowDroppedC4Color;
	}
	
	namespace chams
	{
		extern int chams_list_mode;

		namespace enemy
		{
			extern bool enabled;
			extern bool visible_only;
			extern bool flat;
			extern bool wireframe;
			extern bool health_chams;

			extern int selected_material;

			extern Color color_visible;
			extern Color color_not_visible;
		}

		namespace teammates
		{
			extern bool enabled;
			extern bool visible_only;
			extern bool flat;
			extern bool wireframe;
			extern bool health_chams;

			extern int selected_material;

			extern Color color_visible;
			extern Color color_not_visible;
		}

		namespace localplayer
		{
			extern bool enabled;
			extern bool flat;
			extern bool wireframe;
			extern bool desync_chams;

			extern int desync_chams_mode;
			extern int selected_material;

			extern Color color;
			extern Color desync_color;
		}

		namespace misc
		{
			extern bool weapon_chams;
			extern bool dropped_weapons;
			extern bool nade_chams;
			extern bool dropped_bomb_chams;
			extern bool planted_bomb_chams;
			extern bool dropped_defusekit_chams;
			extern bool accesories_chams;
			extern bool arms_chams;

			extern Color color_weapon_chams;
			extern Color color_dropped_weapons_chams;
			extern Color color_nade_chams;
			extern Color color_dropped_bomb_chams;
			extern Color color_planted_bomb_chams;
			extern Color color_dropped_defusekit_chams;

			extern Color color_he_chams;
			extern Color color_inc_molotov_chams;
			extern Color color_flashbang_chams;
			extern Color color_decoy_chams;
			extern Color color_smoke_chams;
		}
	}
	
	namespace visuals
	{
		extern bool grenade_prediction;
		extern bool player_info_box;
		extern bool grief_box;
		extern bool world_grenades;
		extern bool sniper_crosshair;
		extern bool planted_c4;
		extern bool dropped_weapons;
		extern bool night_mode;
		extern bool pitch;
		extern bool hitmarker;
		extern int hitsound;
		extern bool skychanger;
		extern int skychanger_mode;
		extern bool newstyle;
		extern bool rcs_cross;
		extern bool bullet_tracer;
		extern float radius;
		extern int rcs_cross_mode;
		extern bool spread_cross;
		extern float player_info_box_alpha;
		extern float grief_box_alpha;
		extern int viewmodel_mode;
		extern bool no_sky;
		extern bool no_shadows;

		extern ImVec4 spread_cross_color;
		extern ImVec4 drawfov_color;
		extern Color colorBulletTracer;
		extern ImVec4 recoilcolor;

		extern ImVec4 sky;
	}

	namespace desync
	{
		extern bool enabled;
		extern bool yaw_flip;
		extern bool resolver;
	}

	namespace aimbot
	{
		extern int setting_type;

		extern std::vector<aimbot_group> m_groups;
		extern std::map<int, weapon_setting> m_items;
	}

	namespace misc
	{
		extern bool bhop;
		extern bool auto_strafe;
		extern bool no_flash;
		extern bool no_smoke;
		extern float viewmodel_fov;
		extern bool deathmatch;
		extern bool disable_on_weapon;
		extern bool radar;
		extern int debug_fov;
		extern bool triggerbot_in_zoom_only;
		extern bool lknife;
		extern bool noscope;
		extern bool lefthandknife;
		extern bool equip;
		extern bool damage_indicator;
		extern bool fast_stop;
		extern bool force_inventory_open;
		extern bool anti_obs;
		extern int fast_stop_mode;
		extern bool edge_jump;
		extern bool override_viewmodel;
		extern bool edge_jump_duck_in_air;
		extern bool block_bot;
		extern bool smoke_helper;
		extern bool flash_helper;
		extern bool ow_reveal;

		extern float viewmodel_offset_x;
		extern float viewmodel_offset_y;
		extern float viewmodel_offset_z;
	}
}
