#pragma once

#include "../valve_sdk/csgostructs.hpp"

#include <list>
#include <mutex>

namespace entities
{
	struct hitbox_t
	{
		float radius;

		Vector min;
		Vector max;

		Vector points[4];
	};

	struct local_data_t
	{
		Vector local_pos;
		Vector world_pos;
		Vector eye_pos;
		QAngle angles;
		QAngle punch_angle;

		CHandle<c_base_combat_weapon> active_weapon;
		CCSWeaponInfo* weapon_data;
		
		int tick_base;
		int health;
		int armor_value;
		int hp = 0;
		int damage;
		int bombsite_index;
		int shots_fired;

		float bomb_time = 0.f;
		float defuse_time = 0.f;

		bool has_sniper;
		bool is_scoped;
		bool is_defusing;
		bool is_flashed;
		bool is_alive;
		bool is_bomb_planted;
		bool bomb_has_been_planted;
		bool is_matchmaking = false;

		c_base_player* local;
	};

	struct player_data_t
	{
		int index = 0;
		int m_iHealth;
		int m_iMoney;
		int m_ArmorValue;
		int m_iAmmo;
		int m_MaxAmmo;

		std::string name;
		std::string weapon;
		std::string icon;
		
		float wep_str_size;

		bool is_shooting = false;
		Vector shot_origin;
	
		bool is_enemy;
		bool is_scoped;
		bool is_flashed;
		bool is_defusing;
		bool is_dormant;
		bool is_reloading;
		bool has_helmet;
		bool has_kevlar;
		bool is_c4_carrier;
		bool has_defkit;
		bool is_knife_or_grenade;
		bool is_player;
		bool in_smoke;
		bool is_visible;
		bool is_alive;
		
		int team_num;
		int patches[MAX_PATCHES];

		c_base_player* player;

		float m_flShotTime;
		float m_flSimulationTime;
		
		Vector points[8];

		RECT box;
		Vector eye_pos;
		Vector offset;
		Vector origin;
		Vector old_origin;
		Vector world_pos;
		QAngle angles;

		const model_t* model;
		studiohdr_t* hdr;
	
		hitbox_t hitboxes[HITBOX_MAX];
	};

	struct tick_data_t
	{
		int tick_count;

		player_data_t players[MAX_PLAYERS];
	};

	extern std::mutex locker;
	extern std::mutex local_mutex;

	extern local_data_t m_local;
	extern std::list<tick_data_t> m_items;

	void fetch_targets(CUserCmd* cmd);
}