#pragma once

#include "../valve_sdk/sdk.hpp"
#include "../valve_sdk/csgostructs.hpp"

namespace autowall
{
	struct fire_bullet_data
	{
		Vector src;
		trace_t enter_trace;
		Vector direction;
		CTraceFilter filter;
		float trace_length;
		float trace_length_remaining;
		float current_damage;
		int penetrate_count;
	};

	float get_damage(const Vector& point);
	float get_damage(c_base_player* player, const Vector& vecPoint);
	bool simulate_fire_bullet(c_base_player* ignore, c_base_combat_weapon* pWeapon, fire_bullet_data& data);
	bool handle_bullet_penetration(CCSWeaponInfo* weaponInfo, fire_bullet_data& data);
	bool trace_to_exit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace);
	//void scale_damage(int hitbox, c_base_player* enemy, float weapon_armor_ratio, float& current_damage);
	void scale_damage(int hit_group, c_base_player* enemy, float weapon_armor_ratio, float headshot_multiplier, float& current_damage);
	//void scale_damage(const int iHitGroup, c_base_player* pEntity, const float flWeaponArmorRatio, const float flWeaponHeadShotMultiplier, float& flDamage);
	//void scale_damage(const int& hit_group, const float& weapon_armor_ratio, const bool& has_helmet, const float& armor_value, float& current_damage);
	void trace_line(const Vector& start, const Vector& end, unsigned int mask, c_base_player* ignore, trace_t* ptr);
}