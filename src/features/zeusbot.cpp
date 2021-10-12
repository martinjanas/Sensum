#include "features.h"
#include "../settings/globals.h"
#include "../helpers/input.h"
#include "../helpers/console.h"
#include "../helpers/autowall.h"
#include "../helpers/entities.h"
namespace zeusbot
{
	float range = 150.f;
	c_base_combat_weapon* weapon = nullptr;

	bool is_enabled(CUserCmd* cmd)
	{
		if (!g::local_player || !g::local_player->IsAlive() || cmd->buttons & IN_ATTACK)
			return false;

		if (globals::binds::zeus_bot == 0 || !input_system::is_key_down(globals::binds::zeus_bot))
			return false;

		weapon = g::local_player->m_hActiveWeapon();
		if (!weapon || weapon->m_iItemDefinitionIndex() != WEAPON_TASER || !weapon->CanFire())
			return false;

		return true;
	}

	void handle(CUserCmd* cmd)
	{
		if (!is_enabled(cmd))
			return;

		const auto weapon_data = weapon->GetWeaponData();

		auto best_fov = -1.f;
		auto best_time = 0.f;
		Vector best_pos;

		Ray_t ray;
		QAngle angles;
		CGameTrace tr;
		c_base_player* player;
		c_base_player* target = nullptr;

		auto nci = g::engine_client->GetNetChannelInfo();
		if (!nci)
			return;

		static const auto sv_maxunlag = g::cvar->find("sv_maxunlag");
		const auto unlag = sv_maxunlag->GetFloat();
		const auto interpolation_comp = utils::get_interpolation_compensation();
		const auto network_delay = std::clamp(nci->GetLatency(FLOW_OUTGOING) + nci->GetLatency(FLOW_INCOMING) + interpolation_comp, 0.f, unlag);
		const auto out_delay = std::clamp(nci->GetLatency(FLOW_OUTGOING) + interpolation_comp, 0.f, unlag);;
		const auto correct_next_time = TICKS_TO_TIME(cmd->tick_count + 1) + network_delay;

		CTraceFilterWorldAndPropsOnly filter;
		const auto eye_pos = g::local_player->GetEyePos();
		for (const auto& tick : entities::m_items)
		{
			for (const auto& entity : tick.players)
			{
				if (entity.index == 0)
					continue;

				if (fabsf(out_delay - (correct_next_time - entity.m_flSimulationTime - interpolation_comp)) > 0.2f)
					continue;

				player = c_base_player::GetPlayerByIndex(entity.index);
				if (!player || player == g::local_player)
					continue;

				for (size_t h = 0; h < sizeof(entity.hitboxes); h++)
				{
					if (h == HITBOX_LEFT_FOOT || h == HITBOX_RIGHT_FOOT || h == HITBOX_RIGHT_HAND || h == HITBOX_LEFT_HAND)
						continue;

					const auto hitbox = entity.hitboxes[h][0];
					if (!hitbox.IsValid())
						continue;

					const auto distance = eye_pos.DistTo(hitbox);
					if (distance > weapon_data->flRange)
						continue;

					math::vector2angles(hitbox - eye_pos, angles);
					angles.NormalizeClamp();

					const auto fov = math::GetFovToPlayer(cmd->viewangles, angles);
					if (fov > best_fov && best_fov > 0.f)
						continue;

					//Damage* [RangeModifier ^ (Distance(u) / 500u)]
					const auto damage = weapon_data->iDamage * pow(weapon_data->flRangeModifier, distance / 500);
					if (damage < 100.f && damage < player->m_iHealth())
						continue;

					ray.Init(eye_pos, hitbox);
					g::engine_trace->trace_ray(ray, MASK_SOLID, &filter, &tr);
					if (eye_pos.DistTo(tr.endpos) != distance)
						continue;

					//console::print("zeus damage %.2f", damage);

					target = player;
					best_pos = hitbox;
					best_fov = fov;
					best_time = entity.m_flSimulationTime;
				}
			}
		}

		if (!target)
			return;

		math::vector2angles(best_pos - eye_pos, angles);
		angles.NormalizeClamp();

		if (!utils::hitchance(target, angles, 80.f))
			return;

		cmd->tick_count = TIME_TO_TICKS(best_time + interpolation_comp);

		const auto old_angles = cmd->viewangles;
		cmd->viewangles = angles;
		g::engine_client->SetViewAngles(angles);

		math::correct_movement(cmd, old_angles);

		if (!(cmd->buttons & IN_ATTACK))
			cmd->buttons |= IN_ATTACK;
	}
}