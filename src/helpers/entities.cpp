#include "math.h"
#include "console.h"
#include "entities.h"
#include "../settings/settings.h"
#include "../settings/globals.h"

namespace entities
{
	const auto point_scale = 0.5f;

	ConVar* game_type = nullptr;
	ConVar* game_mode = nullptr;

	std::mutex locker;
	std::mutex local_mutex;

	local_data_t m_local;
	std::list<tick_data_t> m_items(24);
	player_data_t entity_data[MAX_PLAYERS];

	matrix3x4_t bone_matrix[MAXSTUDIOBONES];

	QAngle aim_angles;
	Vector top = Vector(0, 0, 1.f);
	Vector bot = Vector(0, 0, -1.f);
	Vector min, max, forward, origin, head_screen;

	Ray_t ray;
	CGameTrace tr;

	bool is_hitbox_has_multipoints(const int& hitbox)
	{
		switch (hitbox)
		{
		case HITBOX_HEAD:
		case HITBOX_NECK:
		case HITBOX_BELLY:
		case HITBOX_UPPER_CHEST:
			return true;
		default:
			return false;
		}
	}

// 	bool is_hitbox_for_visible_check(const int& hitbox)
// 	{
// 		switch (hitbox) {
// 		case HITBOX_HEAD:
// 		case HITBOX_UPPER_CHEST:
// 		case HITBOX_RIGHT_CALF:
// 		case HITBOX_LEFT_CALF:
// 		case HITBOX_RIGHT_HAND:
// 		case HITBOX_LEFT_HAND:
// 			return true;
// 		default:
// 			return false;
// 		}
// 	}

	bool is_hitbox_for_visible_check(const int& hitbox)
	{
		switch (hitbox) {
		case HITBOX_HEAD:
		case HITBOX_NECK:
		case HITBOX_PELVIS:
		case HITBOX_BELLY:
		case HITBOX_THORAX:
		case HITBOX_LOWER_CHEST:
		case HITBOX_UPPER_CHEST:
		case HITBOX_RIGHT_THIGH:
		case HITBOX_LEFT_THIGH:
		case HITBOX_RIGHT_CALF:
		case HITBOX_LEFT_CALF:
		case HITBOX_RIGHT_FOOT:
		case HITBOX_LEFT_FOOT:
		case HITBOX_RIGHT_HAND:
		case HITBOX_LEFT_HAND:
		case HITBOX_RIGHT_UPPER_ARM:
		case HITBOX_RIGHT_FOREARM:
		case HITBOX_LEFT_UPPER_ARM:
		case HITBOX_LEFT_FOREARM:
			return true;
		default:
			return false;
		}
	}

	RECT GetBBox(c_base_entity* ent, Vector* pointsTransformed)
	{
		auto collideable = ent->GetCollideable();
		if (!collideable)
			return {};

		auto& min = collideable->OBBMins();
		auto& max = collideable->OBBMaxs();

		const matrix3x4_t& trans = ent->m_rgflCoordinateFrame();

		Vector points[] =
		{
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		for (int i = 0; i < 8; i++)
			math::VectorTransform(points[i], trans, pointsTransformed[i]);

		return {};
	}

	float curtime(const int& tick_base)
	{
		return g::global_vars->interval_per_tick * tick_base;
	}

	float damage_for_armor(const float& damage, const int& armor_value) 
	{
		if (armor_value <= 0.f)
			return damage;

		float new_damage = damage * 0.5f;
		float armor = (damage - new_damage) * 0.5f;
		if (armor > static_cast<float>(armor_value))
		{
			armor = static_cast<float>(armor_value) * (1.f / 0.5f);
			new_damage = (damage - armor);
		}

		return new_damage;
	}

	int get_health(c_base_player* local, c_base_planted_c4* bomb)
	{
		float damage;
		float hp_remaining = local->m_iHealth();
		auto distance = local->GetEyePos().DistTo(bomb->m_vecOrigin());
		auto a = 450.7f;
		auto b = 75.68f;
		auto c = 789.2f;
		auto d = ((distance - b) / c);
		auto fl_damage = a * exp(-d * d);
		damage = damage_for_armor(fl_damage, local->m_ArmorValue());
		hp_remaining -= damage;

		return hp_remaining < 0 ? 0 : (hp_remaining + 1);
	}

	float get_bomb_time(c_base_planted_c4* bomb, const int& tick_base)
	{
		if (!bomb->m_bBombTicking())
			return 0;

		const auto bomb_time = bomb->m_flC4Blow() - curtime(tick_base);

		return bomb_time >= 0.f ? bomb_time : 0;
	}

	float get_defuse_time(c_base_planted_c4* bomb, const int& tick_base)
	{
		static float defuse_time = -1.f;

		if (!bomb->m_hBombDefuser())
			defuse_time = -1.f;
		else if (defuse_time == -1.f)
			defuse_time = curtime(tick_base) + bomb->m_flDefuseLength();

		if (defuse_time > -1.f && bomb->m_hBombDefuser())
			return defuse_time - curtime(tick_base);

		return 0.f;
	}

	c_base_planted_c4* get_bomb()
	{
		c_base_entity* entity;

		const auto max_entities = g::entity_list->GetMaxEntities();
		for (auto i = 1; i <= max_entities; ++i)
		{
			entity = c_base_entity::GetEntityByIndex(i);
			if (entity && !entity->IsDormant() && entity->IsPlantedC4())
				return reinterpret_cast<c_base_planted_c4*>(entity);
		}

		return nullptr;
	}

	void destroy()
	{
		local_mutex.lock();
		{
			m_local = local_data_t{};
		}
		local_mutex.unlock();

		locker.lock();
		{
			m_items.clear();
		}
		locker.unlock();
	}

	bool is_matchmaking()
	{
		if (!game_type)
			game_type = g::cvar->find("game_type");

		if (!game_mode)
			game_mode = g::cvar->find("game_mode");

		if (game_type->GetInt() != 0) //classic
			return false;

		if (game_mode->GetInt() != 0 && game_mode->GetInt() != 1 && game_mode->GetInt() != 2) //casual, mm, wingman
			return false;

		return true;
	}

	void fetch_hp(local_data_t& tick_data)
	{
		tick_data.is_matchmaking = is_matchmaking();
		if (!tick_data.is_matchmaking || !tick_data.local)
			return;

		tick_data.hp = tick_data.is_alive ? tick_data.health : 0;

		const auto bomb = get_bomb();

		if (!bomb)
		{
			tick_data.is_bomb_planted = false;
			tick_data.bomb_has_been_planted = false;
		}

		if (bomb && !bomb->m_bBombTicking())
		{
			tick_data.bombsite_index = bomb->m_nBombSite();
			tick_data.is_bomb_planted = bomb->IsPlantedC4();
			tick_data.bomb_has_been_planted = false;
		}

		if (bomb && bomb->m_bBombTicking())
		{
			tick_data.bomb_time = get_bomb_time(bomb, tick_data.tick_base);
			tick_data.defuse_time = get_defuse_time(bomb, tick_data.tick_base);
			tick_data.bombsite_index = bomb->m_nBombSite();
			tick_data.is_bomb_planted = bomb->IsPlantedC4();
			tick_data.bomb_has_been_planted = true;

			tick_data.hp = get_health(tick_data.local, bomb);
		}
	}

	void set_local(c_base_player* local, local_data_t& tick_data, const int& tick_base)
	{
		tick_data.local = local;
		tick_data.angles = local->m_angEyeAngles();
		tick_data.eye_pos = local->GetEyePos();
		tick_data.is_alive = local->IsAlive();
		tick_data.is_flashed = local->IsFlashed();
		tick_data.is_scoped = local->m_bIsScoped();

		tick_data.tick_base = tick_base;

		tick_data.health = local->m_iHealth();
		tick_data.armor_value = local->m_ArmorValue();
		tick_data.shots_fired = local->m_iShotsFired();

		tick_data.has_sniper = tick_data.is_alive && local->m_hActiveWeapon() && local->m_hActiveWeapon()->IsSniper();

		tick_data.world_pos = local->m_vecOrigin();
		math::world2screen(tick_data.world_pos, tick_data.local_pos);

		tick_data.hp = 0;
		tick_data.bomb_time = 0.f;
		tick_data.defuse_time = 0.f;

		tick_data.punch_angle = local->m_aimPunchAngle();
		tick_data.active_weapon = local->m_hActiveWeapon();

		if (tick_data.active_weapon)
			tick_data.weapon_data = tick_data.active_weapon->GetWeaponData();

		fetch_hp(tick_data);
	}

	void get_hitboxes(c_base_player* local, c_base_player* player, studiohdr_t* hdr, const bool& on_screen, bool& in_smoke, bool& is_visible, entities::player_data_t* entity)
	{
		GetBBox(player, entity->points);

		QAngle aim_angles;

		Vector forward;
		Vector top = Vector(0, 0, 1.f);
		Vector bot = Vector(0, 0, -1.f);

		const auto eye_pos = local->GetEyePos();
		CTraceFilterSkipTwoEntities filter(local, player);

		for (int k = HITBOX_HEAD; k < HITBOX_MAX; k++)
		{
			const auto hitbox = hdr->GetHitboxSet(0)->GetHitbox(k);
			if (!hitbox)
				continue;

			const auto bone = bone_matrix[hitbox->bone];
			const auto mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

			math::VectorTransform(hitbox->bbmax + mod, bone, max);
			math::VectorTransform(hitbox->bbmin - mod, bone, min);

			auto& pos = entity->hitboxes[k] = { mod, min, max, { (min + max) * 0.5f } };

			if (is_hitbox_has_multipoints(k))
			{
				math::vector2angles(pos.points[0] - eye_pos, aim_angles);
				aim_angles.NormalizeClamp();

				math::angle2vectors(aim_angles, forward);

				Vector right = forward.Cross(Vector(0, 0, 1));
				Vector left = Vector(-right.x, -right.y, right.z);

				const auto index = k == HITBOX_HEAD ? 2 : 1;
				const auto custom_scale = k == HITBOX_HEAD ? 0.7f : point_scale;

				if (k == HITBOX_HEAD)
					pos.points[1] = pos.points[0] + top * (mod * custom_scale);

				pos.points[index] = pos.points[0] + right * (mod * custom_scale);
				pos.points[index + 1] = pos.points[0] + left * (mod * custom_scale);
			}

			if (!on_screen || !is_hitbox_for_visible_check(k))
				continue;

			if (!is_visible)
			{
				ray.Init(eye_pos, pos.points[0]);
				g::engine_trace->trace_ray(ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX, &filter, &tr);

				is_visible = tr.endpos.DistTo(eye_pos) == pos.points[0].DistTo(eye_pos);
			}
			
			if (!utils::is_line_goes_through_smoke(eye_pos, pos.points[0]))
				in_smoke = false;
		}
	}

	void fetch_targets(CUserCmd* cmd)
	{
		c_base_player* local = g::local_player;
		if (!local)
		{
			destroy();
			return;
		}

 		if (!g::local_player->IsAlive() && g::local_player->m_hObserverTarget())
 		{
 			const auto observer = (c_base_player*)c_base_player::GetEntityFromHandle(g::local_player->m_hObserverTarget());
 			if (observer && observer->IsPlayer())
 				local = observer;
 		}

		local_mutex.lock();
		set_local(local, m_local, g::local_player->m_nTickBase());
		local_mutex.unlock();
		
		std::unique_ptr<tick_data_t> tick_data = std::unique_ptr<tick_data_t>(new tick_data_t());
		tick_data->tick_count = cmd->tick_count;

		const auto eye_pos = local->GetEyePos();

		bool in_smoke;
		bool is_visible;

		c_base_player* player;

		const auto max_clients = g::engine_client->GetMaxClients();
		for (int i = 1; i <= max_clients; i++)
		{
			player = c_base_player::GetPlayerByIndex(i);
			if (!player || player == local)
				continue;

			if (settings::misc::radar && !player->m_bSpotted())
				player->m_bSpotted() = true;
			else if (!settings::misc::radar && !player->m_bSpotted() && utils::is_entity_audible(player->GetIndex(), player->m_vecOrigin()) || player->HasC4())
				player->m_bSpotted() = true;

			if (!player->IsPlayer() || player->IsDormant() || player->m_iHealth() <= 0 || !player->IsAlive())
				continue;

			if (player->m_fFlags() & FL_FROZEN || player->m_bGunGameImmunity())
				continue;

			const auto is_enemy = player->m_iTeamNum() != local->m_iTeamNum();
			if (!is_enemy && !settings::misc::deathmatch)
				continue;

			auto studio_model = g::mdl_info->GetStudiomodel(player->GetModel());
			if (!studio_model)
				continue;

			std::unique_ptr<player_data_t> player_data = std::unique_ptr<player_data_t>(new player_data_t());
			player_data->index = player->GetIndex();
			player_data->world_pos = player->m_vecOrigin();
			player_data->eye_pos = player->GetEyePos();

			player_data->name = std::string(player->GetPlayerInfo().szName).substr(0, 12);

			auto& active_wpn = player->m_hActiveWeapon();

			if (!active_wpn)
				continue;

			player_data->weapon = utils::get_weapon_name(active_wpn);
			player_data->icon = active_wpn.Get()->GetGunIcon();
			player_data->wep_str_size = active_wpn.Get()->GetGunStringSize();
			player_data->is_knife_or_grenade = (active_wpn->IsKnifeOrGrenade());

			player_data->is_dormant = player->IsDormant();
			player_data->is_scoped = player->m_bIsScoped();
			player_data->is_flashed = player->IsFlashed();
			player_data->is_defusing = player->m_bIsDefusing();
			player_data->has_helmet = player->m_bHasHelmet();
			player_data->has_kevlar = player->m_ArmorValue() > 0;
			player_data->is_c4_carrier = player->HasC4();
			player_data->has_defkit = player->m_bHasDefuser();
			player_data->player = player;
			player_data->model = player->GetModel();
			player_data->is_player = player->IsPlayer();
			player_data->team_num = player->m_iTeamNum();
			player_data->hdr = g::mdl_info->GetStudiomodel(player_data->model);
			player_data->is_alive = player->IsAlive();
		
			player_data->is_enemy = true;
			player_data->m_iMoney = player->m_iAccount();
			player_data->m_iHealth = player->m_iHealth();
			player_data->m_ArmorValue = player->m_ArmorValue();
			player_data->m_flSimulationTime = player->m_flSimulationTime();
			player_data->m_iAmmo = active_wpn.Get()->m_iClip1();

			player_data->m_MaxAmmo = active_wpn.Get()->m_iPrimaryReserveAmmoCount();
			const auto tick_offset = player->m_vecVelocity() * g::global_vars->interval_per_tick;
			
			in_smoke = true;
			is_visible = false;

			const auto on_screen = math::world2screen(player->m_vecOrigin(), origin);

			const auto& abs_origin = player->GetAbsOrigin();
			
			player->SetAbsOrigin(player_data->world_pos);

			player->InvalidateBoneCache();
			
			if (!player->SetupBones(bone_matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, g::global_vars->curtime))
			{
				player->SetAbsOrigin(abs_origin);
				player->InvalidateBoneCache();

				continue;
			}
			
			get_hitboxes(local, player, studio_model, on_screen, in_smoke, is_visible, player_data.get());
			
			/*CTraceFilterSkipTwoEntities filter(local, player);
			for (int k = HITBOX_HEAD; k < HITBOX_MAX; k++)
			{
				const auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(k);
				if (!hitbox)
					continue;

				const auto bone = bone_matrix[hitbox->bone];
				const auto mod = hitbox->m_flRadius != -1.f ? hitbox->m_flRadius : 0.f;

				math::VectorTransform(hitbox->bbmax + mod, bone, max);
				math::VectorTransform(hitbox->bbmin - mod, bone, min);

				auto& pos = player_data->hitboxes[k] = { mod, min, max, { (min + max) * 0.5f } };

				if (is_hitbox_has_multipoints(k))
				{
					math::vector2angles(pos.points[0] - eye_pos, aim_angles);
					aim_angles.NormalizeClamp();

					math::angle2vectors(aim_angles, forward);

					Vector right = forward.Cross(Vector(0, 0, 1));
					Vector left = Vector(-right.x, -right.y, right.z);

					const auto index = k == HITBOX_HEAD ? 2 : 1;
					const auto custom_scale = k == HITBOX_HEAD ? 0.7f : point_scale;

					if (k == HITBOX_HEAD)
						pos.points[1] = pos.points[0] + top * (mod * custom_scale);

					pos.points[index] = pos.points[0] + right * (mod * custom_scale);
					pos.points[index + 1] = pos.points[0] + left * (mod * custom_scale);
				}

				if (!on_screen || !is_hitbox_for_visible_check(k))
					continue;

				if (!is_visible)
				{
					ray.Init(eye_pos, pos.points[0]);
					g::engine_trace->trace_ray(ray, CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX, &filter, &tr);

					is_visible = tr.endpos.DistTo(eye_pos) == pos.points[0].DistTo(eye_pos);
				}

				if (is_visible && in_smoke && !utils::is_line_goes_through_smoke(eye_pos, pos.points[0]))
					in_smoke = false;
			}*/

			player->SetAbsOrigin(abs_origin);

			player_data->in_smoke = in_smoke;
			player_data->is_visible = is_visible;

			globals::is_visible[player->GetIndex()] = player_data->is_visible;
			globals::in_smoke[player->GetIndex()] = player_data->in_smoke;

			player_data->box = GetBBox(player, player_data->points);
			player_data->origin = player->m_vecOrigin();
			player_data->old_origin = origin;
			player_data->offset = tick_offset;
			player_data->angles = player->m_angEyeAngles();

			tick_data->players[i] = *player_data;
		}

		locker.lock();
		{
			m_items.push_front(*tick_data);
			m_items.resize(13);
		}
		locker.unlock();
	}
}