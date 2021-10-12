#include "math.h"
#include "utils.h"
#include "console.h"
#include "autowall.h"
#include "../settings/settings.h"
#include "../valve_sdk/interfaces/IEngineTrace.hpp"

#include <math.h>

#define HITGROUP_GENERIC	0
#define HITGROUP_HEAD		1
#define HITGROUP_CHEST		2
#define HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

namespace autowall
{
	void scale_damage(const int& hit_group, const float& weapon_armor_ratio, const bool& has_helmet, const float& armor_value, float& current_damage)
	{
		switch (hit_group)
		{
		case HITGROUP_HEAD:
			current_damage *= 4.f;
			break;
		case HITGROUP_STOMACH:
			current_damage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			current_damage *= 0.75f;
			break;
		}

		const auto armor = armor_value;
		if (armor > 0)
		{
			float ratio = 0.f;
			switch (hit_group)
			{
			case HITGROUP_HEAD:
				if (has_helmet)
				{
					ratio = (weapon_armor_ratio * 0.5) * current_damage;
					if (((current_damage - ratio) * 0.5) > armor)
					{
						ratio = current_damage - (armor * 2.0);
					}
					current_damage = ratio;
				}

				break;
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				ratio = (weapon_armor_ratio * 0.5) * current_damage;
				if (((current_damage - ratio) * 0.5) > armor)
				{
					ratio = current_damage - (armor * 2.0);
				}
				current_damage = ratio;
				break;
			}
		}
	}

	void scale_damage(int hit_group, c_base_player* enemy, float weapon_armor_ratio, float& current_damage)
	{
		scale_damage(hit_group, weapon_armor_ratio, enemy->m_bHasHelmet(), enemy->m_ArmorValue(), current_damage);
	}

	bool trace_to_exit(Vector& end, trace_t* enter_trace, Vector start, Vector dir, trace_t* exit_trace)
	{
		float distance = 0.0f;

		while (distance <= 90.0f)
		{
			distance += 4.0f;
			end = start + dir * distance;

			auto point_contents = g::engine_trace->GetPointContents(end, MASK_SHOT_HULL | CONTENTS_HITBOX, NULL);

			if (point_contents & MASK_SHOT_HULL && !(point_contents & CONTENTS_HITBOX))
				continue;

			auto new_end = end - (dir * 4.0f);

			Ray_t ray;
			ray.Init(end, new_end);
			g::engine_trace->trace_ray(ray, MASK_SHOT, 0, exit_trace);

			if (exit_trace->startsolid && exit_trace->surface.flags & SURF_HITBOX)
			{
				ray.Init(end, start);

				CTraceFilter filter;
				filter.pSkip = exit_trace->hit_entity;

				g::engine_trace->trace_ray(ray, 0x600400B, &filter, exit_trace);

				if ((exit_trace->fraction < 1.0f || exit_trace->allsolid) && !exit_trace->startsolid)
				{
					end = exit_trace->endpos;
					return true;
				}

				continue;
			}

			if (!(exit_trace->fraction < 1.0 || exit_trace->allsolid || exit_trace->startsolid) || exit_trace->startsolid)
			{
				if (exit_trace->hit_entity)
				{
					if (enter_trace->hit_entity && enter_trace->hit_entity == g::entity_list->GetClientEntity(0))
						return true;
				}

				continue;
			}

			if (exit_trace->surface.flags >> 7 & 1 && !(enter_trace->surface.flags >> 7 & 1))
				continue;

			if (exit_trace->plane.normal.Dot(dir) <= 1.0f)
			{
				auto fraction = exit_trace->fraction * 4.0f;
				end = end - dir * fraction;

				return true;
			}
		}

		return false;
	}

	bool handle_bullet_penetration(CCSWeaponInfo* weapon_data, fire_bullet_data& data)
	{
		surfacedata_t* enter_surface_data = g::physics_surface->GetSurfaceData(data.enter_trace.surface.surfaceProps);
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;

		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= powf(weapon_data->flRangeModifier, data.trace_length * 0.002f);

		if (data.trace_length > 3000.f || enter_surf_penetration_mod < 0.1f)
			data.penetrate_count = 0;

		if (data.penetrate_count <= 0)
			return false;

		Vector dummy;
		trace_t trace_exit;

		if (!trace_to_exit(dummy, &data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
			return false;

		surfacedata_t* exit_surface_data = g::physics_surface->GetSurfaceData(trace_exit.surface.surfaceProps);
		int exit_material = exit_surface_data->game.material;

		float exit_surf_penetration_mod = *(float*)((uint8_t*)exit_surface_data + 76);
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;

		if ((data.enter_trace.contents & CONTENTS_GRATE) != 0 || enter_material == 89 || enter_material == 71)
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;

		if (enter_material == exit_material)
		{
			if (exit_material == 87 || exit_material == 85)
				combined_penetration_modifier = 3.0f;
			else if (exit_material == 76)
				combined_penetration_modifier = 2.0f;
		}

		float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / weapon_data->flPenetration) * 1.25f);
		float thickness = (trace_exit.endpos - data.enter_trace.endpos).Length();

		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;

		float lost_damage = fmaxf(0.0f, v35 + thickness);

		if (lost_damage > data.current_damage)
			return false;

		if (lost_damage >= 0.0f)
			data.current_damage -= lost_damage;

		if (data.current_damage < 1.0f)
			return false;

		data.src = trace_exit.endpos;
		data.penetrate_count--;

		return true;
	}

	void trace_line(const Vector& start, const Vector& end, unsigned int mask, c_base_player* ignore, trace_t* ptr)
	{
		Ray_t ray;
		ray.Init(start, end);
		CTraceFilter filter;
		filter.pSkip = ignore;

		g::engine_trace->trace_ray(ray, mask, &filter, ptr);
	}

	bool simulate_fire_bullet(c_base_player* ignore, c_base_combat_weapon* weapon, fire_bullet_data& data)
	{
		auto weapon_data = weapon->GetWeaponData();

		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		data.current_damage = float(weapon_data->iDamage);

		while (data.penetrate_count > 0 && data.current_damage >= 1.0f)
		{
			data.trace_length_remaining = weapon_data->flRange - data.trace_length;
			Vector end = data.src + data.direction * data.trace_length_remaining;

			trace_line(data.src, end, MASK_SHOT, ignore, &data.enter_trace); //was commented

			Ray_t ray; //was commented
			ray.Init(data.src, end + data.direction * 40.f); //was commented

			g::engine_trace->trace_ray(ray, MASK_SHOT, &data.filter, &data.enter_trace); //was commented

			trace_line(data.src, end + data.direction * 40.f, MASK_SHOT, ignore, &data.enter_trace);

			if (data.enter_trace.fraction == 1.0f)
				break;

			if (data.enter_trace.hitgroup <= HITGROUP_RIGHTLEG && data.enter_trace.hitgroup > HITGROUP_GENERIC)
			{
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= powf(weapon_data->flRangeModifier, data.trace_length * 0.002f);

				auto player = (c_base_player*)data.enter_trace.hit_entity;
				if (player && player->m_iTeamNum() == ignore->m_iTeamNum() && !settings::misc::deathmatch)
					break;

				scale_damage(data.enter_trace.hitgroup, player, weapon_data->flArmorRatio, data.current_damage);

				return true;
			}

			if (!handle_bullet_penetration(weapon_data, data))
				break;
		}

		return false;
	}

	float get_damage(const Vector& point)
	{
		return get_damage(g::local_player, point);
	}

	float get_damage(c_base_player* player, const Vector& point)
	{
		float damage = 0.f;

		auto weapon = player->m_hActiveWeapon();
		if (!weapon || !weapon->IsWeapon())
			return -1.0f;

		fire_bullet_data data;

		data.src = player->GetEyePos();
		data.filter.pSkip = player;

		data.direction = point - data.src;
		data.direction.NormalizeInPlace();

		if (simulate_fire_bullet(player, weapon, data))
			damage = data.current_damage;

		return damage;
	}
}