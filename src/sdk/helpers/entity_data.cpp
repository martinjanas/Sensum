#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"
#include "../../features/features.h"
#include "../classes/CPlantedC4.h"
#include "../classes/CGrenadeProjectile.h"
#include <queue>
#include "../../render/render.h"
#include "../cheat.h"

namespace entity_data
{
	std::list<EntityInstance_t> player_instances;
	std::list<EntityInstance_t> entity_instances;
	std::list<player_entry_data_t> player_entry_data;
	std::list<entity_entry_data_t> entity_entry_data;

	std::mutex player_locker;
	std::mutex entity_locker;

	namespace view_matrix
	{
		VMatrix* matrix;
	};

	void destroy()
	{
		player_entry_data.clear();
	}
	
	void set_bones_w2s(entity_data::player_data_t& data)
	{
		if (!data.bones_w2s.empty())
			return;

		const auto& model = data.m_hModel;
		if (!model.IsValid())
			return;

		const auto& model_state = data.m_ModelState;

		const Vector neck_chest_delta = model_state.bones[EBones::BONE_NECK].position - model_state.bones[EBones::BONE_SPINE_3].position;
		const Vector chest_neck_midpoint = model_state.bones[EBones::BONE_SPINE_3].position + (neck_chest_delta * 0.5f);

		bone_info_t esp_data;
		for (int i = 0; i < EBones::BONE_MAX; ++i)
		{
			const auto& flag = model->GetBoneFlags(i);
			if (!flag.HasFlag(static_cast<uint32_t>(FLAG_HITBOX)))
				continue;

			const auto& bone_parent_index = model->GetBoneParent(i);
			if (bone_parent_index == -1)
				continue;

			const auto& bones = model_state.bones[i];
			const auto& parent_bones = model_state.bones[bone_parent_index];

			Vector bone_pos = bones.position;
			Vector parent_pos = parent_bones.position;

			Vector delta_child = bones.position - chest_neck_midpoint;
			Vector delta_parent = parent_bones.position - chest_neck_midpoint;

			if (delta_parent.length() < 9.0f && delta_child.length() < 9.0f)
				parent_pos = chest_neck_midpoint;

			if (i == EBones::BONE_SPINE_2)
				bone_pos = chest_neck_midpoint;

			if (abs(delta_child.z) < 5.0f && delta_parent.length() < 5.0f && delta_child.length() < 5.0f || i == EBones::BONE_SPINE_3)
				continue;

			esp_data.got_bone = globals::world2screen(bone_pos, esp_data.bone);
			esp_data.got_parent = globals::world2screen(parent_pos, esp_data.bone_parent);

			data.bones_w2s.push_back(esp_data);
		}
	}
	
	bool get_collision_bbox(CGameSceneNode* scene_node, CCollisionProperty* collision, BBox_t& out)
	{
		Vector mins = collision->m_vecMins();
		Vector maxs = collision->m_vecMaxs();

		const matrix3x4_t& matrix = scene_node->m_nodeToWorld().ToMatrix3x4();

		bool valid = true;

		// Start with extreme values for min/max
		Vector screen_min(FLT_MAX, FLT_MAX);
		Vector screen_max(-FLT_MAX, -FLT_MAX);

		for (int i = 0; i < 8; i++)
		{
			Vector point = Vector{
				i & 1 ? maxs.x : mins.x,
				i & 2 ? maxs.y : mins.y,
				i & 4 ? maxs.z : mins.z
			}.transform(matrix);

			// Convert to screen space
			valid &= globals::world2screen(point, out.m_Vertices[i]);

			// Update bounding box limits
			screen_min.x = (std::min)(screen_min.x, out.m_Vertices[i].x);
			screen_min.y = (std::min)(screen_min.y, out.m_Vertices[i].y);
			screen_max.x = (std::max)(screen_max.x, out.m_Vertices[i].x);
			screen_max.y = (std::max)(screen_max.y, out.m_Vertices[i].y);
		}

		if (!valid)
			return false;

		out.m_Mins = screen_min;
		out.m_Maxs = screen_max;

		return true;
	}

	//TODO: Move to utils
	bool is_in_smoke(const Vector& start, const Vector& end, const float& max_density)
	{
		using fn = float(__fastcall*)(const Vector&, const Vector&, void*);
		static auto addr = modules::client.get_sig_addr(FNV("IsInSmoke"), __FUNCTION__).as();
		if (!addr)
			return false;

		auto get_smoke_density = reinterpret_cast<fn>(addr);
		if (get_smoke_density)
		{
			float smoke_density = get_smoke_density(start, end, nullptr);

			return smoke_density >= max_density;
		}
	}

	std::map<int, std::vector<int>> hitbox_hierarchy = 
	{
		{HITBOX_HEAD, {HITBOX_NECK}},
		{HITBOX_NECK, {HITBOX_UPPER_CHEST}},
		{HITBOX_UPPER_CHEST, {HITBOX_LOWER_CHEST}},
		{HITBOX_LOWER_CHEST, {HITBOX_THORAX}},
		{HITBOX_THORAX, {HITBOX_BELLY}},
		{HITBOX_BELLY, {HITBOX_PELVIS}},
		{HITBOX_LEFT_UPPER_ARM, {HITBOX_LEFT_FOREARM}},
		{HITBOX_RIGHT_UPPER_ARM, {HITBOX_RIGHT_FOREARM}},
		{HITBOX_LEFT_THIGH, {HITBOX_LEFT_CALF}},
		{HITBOX_RIGHT_THIGH, {HITBOX_RIGHT_CALF}},
	};

	void set_children_as_visible(int index, entity_data::player_data_t& player_data)
	{
		std::vector<int> to_process;
		to_process.push_back(index);

		while (!to_process.empty())
		{
			int current = to_process.back();
			to_process.pop_back();

			if (hitbox_hierarchy.find(current) == hitbox_hierarchy.end())
				continue;

			for (int child_index : hitbox_hierarchy[current])
			{
				if (player_data.hitboxes[child_index].visible)
					continue;

				player_data.hitboxes[child_index].visible = true;
				to_process.push_back(child_index);
			}
		}
	}

	void update_visibility(entity_data::player_data_t& player_data, CCSPlayerPawn* local_pawn) 
	{
		static Ray_t ray;
		TraceFilter_t filter(MASK_PLAYER_VISIBLE, local_pawn, nullptr, 4);
		Trace_t trace;

		static std::vector<int> hitbox_scan_ids = { HITBOX_HEAD, HITBOX_LOWER_CHEST, HITBOX_BELLY, HITBOX_LEFT_UPPER_ARM, HITBOX_RIGHT_UPPER_ARM, HITBOX_LEFT_THIGH, HITBOX_RIGHT_THIGH };

		for (int hitbox_id : hitbox_scan_ids) 
		{
			auto& hitbox = player_data.hitboxes[hitbox_id];
			if (hitbox.visible)
				continue;

			cheat::interfaces().engine_trace->TraceShape(&ray, local_pawn->GetEyePos(), hitbox.hitbox_pos, &filter, &trace);

			if (!trace.m_pHitEntity || trace.m_pHitEntity == local_pawn || !trace.m_pHitEntity->IsPawn())
				continue;

			bool is_visible = trace.m_pHitEntity == player_data.m_PlayerPawn || trace.m_flFraction == 1.0f/*>= 0.97f*/;

			if (!player_data.flags.test(PLAYER_VISIBLE) && is_visible)
				player_data.flags.set(PLAYER_VISIBLE);

			hitbox.visible = is_visible;

			if (is_visible)
				set_children_as_visible(hitbox_id, player_data);

			if (is_visible && !player_data.flags.test(PLAYER_IN_SMOKE) && is_in_smoke(local_pawn->GetEyePos(), hitbox.hitbox_pos, 0.2f))
				player_data.flags.set(PLAYER_IN_SMOKE);
		}
	}

	void get_hitboxes(entity_data::player_data_t& player_data, const Vector& eye_pos, CCSPlayerPawn* local_pawn, const bool& on_screen)
	{
		HitboxSet_t* hitbox_set = player_data.m_PlayerPawn->GetHitboxSet(0);
		if (!hitbox_set)
			return;

		auto& hitboxes = hitbox_set->m_HitBoxes();
		if (hitboxes.Count() == 0 || hitboxes.Count() > HITBOX_MAX)
			return;
	
		hitbox_info_t hitbox_info;
		for (int i = 0; i < HITBOX_MAX; ++i)
		{
			Hitbox_t* hitbox = &hitboxes[i];

			if (!hitbox)
				continue;
				
			if (!player_data.m_PlayerPawn->HitboxToWorldTransform(hitbox_set, player_data.hitbox_transform))
				continue;

			const auto& hitbox_matrix = player_data.hitbox_transform[i];

			const auto& radius = hitbox->m_flShapeRadius();

			auto min_bounds = hitbox->m_vMinBounds() - radius;
			auto max_bounds = hitbox->m_vMaxBounds() + radius;

			/*auto min_bounds = hitbox->m_vMinBounds();
			auto max_bounds = hitbox->m_vMaxBounds();*/

			const auto& mins = min_bounds.transform(hitbox_matrix.ToMatrix3x4());
			const auto& maxs = max_bounds.transform(hitbox_matrix.ToMatrix3x4());

			auto hitbox_pos = (mins + maxs) * 0.5f;

			player_data.hitboxes[i] = { hitbox_pos, hitbox->m_nHitBoxIndex(), false };
		}

		if (!on_screen)
			return;

		update_visibility(player_data, local_pawn);
	}

	void fetch_player_data()
	{
		if (!cheat::interfaces().engine->IsInGame())
			return;
	
		std::lock_guard<std::mutex> lock(player_locker);

		const auto& local_controller = cheat::interfaces().entity_system->GetLocalPlayerController<CCSPlayerController*>();
		if (!local_controller)
			return;

		CCSPlayerPawn* localpawn =cheat::interfaces().entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPawn());
		if (!localpawn)
		{
			destroy();
			return;
		}

		bool is_localpawn_spectating = !localpawn->IsAlive() && localpawn->m_pObserverServices();
		if (is_localpawn_spectating)
		{
			const auto& observer_controller = cheat::interfaces().entity_system->GetEntityFromHandle<CCSPlayerController*>(localpawn->m_pObserverServices()->m_hObserverTarget());
			if (!observer_controller)
				return;

			CCSPlayerPawn* observer_pawn = reinterpret_cast<CCSPlayerPawn*>(observer_controller);
			if (!observer_pawn)
				return;

			localpawn = observer_pawn;
		}

		const auto& local_team = localpawn->m_iTeamNum();
		const auto& eye_pos = localpawn->GetEyePos();
		
		player_entry_data_t entry_data;
		for (const auto& instance : player_instances)
		{
			if (!instance.entity || !instance.handle.IsValid())
				continue;

			const auto& controller = reinterpret_cast<CCSPlayerController*>(instance.entity);
			if (!controller)
				continue;

			const uint32_t& index = instance.handle.GetEntryIndex();
			if (index <= 0 || index > 0x7FFF)
				continue;

			if (!controller->m_hPawn().IsValid())
				continue;

			const auto& pawn = cheat::interfaces().entity_system->GetEntityFromHandle<CCSPlayerPawn*>(controller->m_hPawn());
			if (!pawn || !pawn->IsAlive() || pawn->m_bGunGameImmunity() || pawn == localpawn || (!cheat::interfaces().game_type->IsDeathmatch() && pawn->m_iTeamNum() == local_team))
				continue;

			const auto& hitbox_set = pawn->GetHitboxSet(0);
			if (!hitbox_set)
				continue;

			const auto& scene_node = pawn->m_pGameSceneNode();
			const auto& weapon_services = pawn->m_pWeaponServices();
			if (!scene_node || !weapon_services)
				continue;

			const auto& active_wpn_handle = weapon_services->m_hActiveWeapon();
			if (!active_wpn_handle.IsValid())
				continue;

			const auto& active_wpn = cheat::interfaces().entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(active_wpn_handle); //weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon*>();
			const auto& collision = pawn->m_pCollision();
			const auto& skeleton_instance = scene_node->GetSkeletonInstance();
			if (!active_wpn || !collision || !skeleton_instance)
				continue;

			const auto& model_state = skeleton_instance->m_modelState();
			const auto& model = model_state.modelHandle;
			if (!model.IsValid())
				continue;

			Vector origin_w2s;
			const auto on_screen = globals::world2screen(scene_node->m_vecOrigin(), origin_w2s);

			player_data_t player_data;
			player_data.m_szPlayerName = controller->m_sSanitizedPlayerName();
			player_data.m_iPlayerIndex = index;
			player_data.m_vecOrigin = scene_node->m_vecOrigin();
			player_data.m_vecAbsOrigin = scene_node->m_vecAbsOrigin();
			player_data.m_vecEyePos = (scene_node->m_vecOrigin() + pawn->m_vecViewOffset());
			player_data.m_vecEyeAngles = pawn->m_angEyeAngles();
			player_data.m_iHealth = pawn->m_iHealth();
			player_data.m_iShotsFired = pawn->m_iShotsFired();
			player_data.m_iClip1 = active_wpn->m_iClip1();
			player_data.m_ModelState = model_state;
			player_data.m_hModel = model;
			player_data.m_PlayerPawn = pawn;
			player_data.distance = player_data.m_vecOrigin.dist_to(localpawn->m_pGameSceneNode()->m_vecOrigin());
			player_data.sim_time = pawn->m_flSimulationTime();

			const EIconType& type = icon_fetcher::get_icon_type_by_weapon_index(active_wpn->m_iItemDefinitionIndex());
			player_data.weapon_icon = icon_fetcher::get(type);

			if (active_wpn->IsKnife())
				player_data.weapon_icon.SetScale(0.45f);
			else if (active_wpn->IsSniper())
				player_data.weapon_icon.SetScale(0.4f);
			else if (active_wpn->IsPistol())
				player_data.weapon_icon.SetScale(0.45f);
			else player_data.weapon_icon.SetScale(0.4f);

			player_data.flags.reset();

			pawn->m_iHealth() > 0 ? player_data.flags.set(PLAYER_ALIVE) : player_data.flags.reset(PLAYER_ALIVE);
			pawn->InAir() ? player_data.flags.set(PLAYER_IN_AIR) : player_data.flags.reset(PLAYER_IN_AIR);

			if (player_data.m_vecOldOrigin != scene_node->m_vecOrigin() || (player_data.m_vecOldEyeAngles.pitch != pawn->m_angEyeAngles().pitch || player_data.m_vecOldEyeAngles.yaw != pawn->m_angEyeAngles().yaw))
			{
				get_hitboxes(player_data, eye_pos, localpawn, on_screen);
				get_collision_bbox(scene_node, collision, player_data.bbox);
				set_bones_w2s(player_data);
			}

			player_data.m_vecOldOrigin = scene_node->m_vecOrigin();
			player_data.m_vecOldEyeAngles = pawn->m_angEyeAngles();

			entry_data.player_data.push_back(player_data);
		}

		entry_data.player_data.sort([](const player_data_t& a, const player_data_t& b) { return a.distance > b.distance; });
		
		player_entry_data.clear();
		player_entry_data.push_back(entry_data);
	}

	#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / 0.015625 ) )
    #define TICKS_TO_TIME(t) ( 0.015625 * (t) )

	void calculate_smoke_time(smoke_info_t& data)
	{
		static float expire_time = 21.f;
		auto spawn_time = (float)TICKS_TO_TIME(data.m_nSmokeEffectTickBegin);
		float total_time_alive =cheat::interfaces().global_vars->m_curtime - spawn_time;
					
		float time_remaining = expire_time - total_time_alive;
		time_remaining = std::clamp(time_remaining, 0.f, 21.f);
					
		data.remaining_smoke_time = time_remaining;
	}
	
	void fetch_entity_info()
	{
		if (!cheat::interfaces().engine->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(entity_locker);

		entity_entry_data_t entry_data;
		for (const auto& instance : entity_instances)
		{
			if (!instance.entity || !instance.handle.IsValid())
				continue;

			const auto& entity = reinterpret_cast<CBaseEntity*>(instance.entity);
			if (!entity)
				continue;

			const uint32_t& index = instance.handle.GetEntryIndex();
			if (index <= 0 || index > 0x7FFF)
				continue;

			if (entity->IsPlantedC4())
			{
				auto bomb_entity = reinterpret_cast<CPlantedC4*>(entity);

				bomb_info_t data;
				data.m_bBombTicking = bomb_entity->m_bBombTicking();
				data.m_flC4Blow = bomb_entity->m_flC4Blow();
				data.m_nBombSite = bomb_entity->m_nBombSite();
				data.m_vecAbsOrigin = entity->m_pGameSceneNode()->m_vecAbsOrigin();

				data.bomb_icon = icon_fetcher::get(EIconType::PLANTED_C4);

				entry_data.bomb_info.push_back(std::move(data));
			}

			if (entity->IsGrenadeProjectile()) 
			{
				auto grenade_entity = reinterpret_cast<CGrenadeProjectile*>(entity);

				grenade_info_t data;
				data.m_vecOrigin = grenade_entity->m_pGameSceneNode()->m_vecOrigin();
				
				if (grenade_entity->IsSmokeProjectile())
				{
					auto smoke_entity = reinterpret_cast<CSmokeProjectile*>(grenade_entity);
					data.type = GRENADE_SMOKE;

					data.icon = icon_fetcher::get(EIconType::SMOKE);
					data.icon.SetScale(1.2f);
					
					smoke_info_t smoke_info;
					smoke_info.m_nSmokeEffectTickBegin = smoke_entity->m_nSmokeEffectTickBegin();
					smoke_info.m_bDidSmokeEffect = smoke_entity->m_bDidSmokeEffect();
					calculate_smoke_time(smoke_info);
					
					data.specific_data = smoke_info;
				}

				if (grenade_entity->IsMolotovProjectile())
				{
					data.type = GRENADE_MOLOTOV;

					data.icon = icon_fetcher::get(EIconType::MOLOTOV);
					data.icon.tint = ImVec4(1.f, 0.49f, 0.15f, 1.f); //orange color
				}

				if (grenade_entity->IsFlashProjectile())
				{
					data.type = GRENADE_FLASH;

					data.icon = icon_fetcher::get(EIconType::FLASHBANG);
					data.icon.tint = ImVec4(1.f, 1.f, 0.f, 1.f); //yellow color	
				}

				if (grenade_entity->IsHEProjectile())
				{
					auto he_entity = reinterpret_cast<CHEGrenadeProjectile*>(grenade_entity);
					data.type = GRENADE_HE;

					data.icon = icon_fetcher::get(EIconType::HEGRENADE);
					data.icon.SetScale(0.7f);
					data.icon.tint = ImVec4(1.f, 0.f, 0.f, 1.f);	
					
					hegrenade_info_t he_info;
					he_info.m_nExplodeEffectTickBegin = he_entity->m_nExplodeEffectTickBegin();

					data.specific_data = he_info;
				}

				if (grenade_entity->IsDecoyProjectile())
				{
					data.type = GRENADE_DECOY;

					data.icon = icon_fetcher::get(EIconType::DECOY);
					data.icon.tint = ImVec4(0.f, 1.f, 0.f, 1.f);	
				}
				
				entry_data.grenade_info.push_back(data);
			}

			//Only push entities that do not have player owner
			if (entity->IsDroppedWorldEntity() && !entity->m_hOwnerEntity().IsValid())
			{
				auto dropped_entity = reinterpret_cast<CBaseEntity*>(entity);

				world_entity_info_t data;
				data.m_vecOrigin = dropped_entity->m_pGameSceneNode()->m_vecOrigin();
				data.name = dropped_entity->m_pEntity()->m_designerName();
				
				if (auto weapon_entity = reinterpret_cast<CBasePlayerWeapon*>(dropped_entity); weapon_entity)
				{
					uint16_t weapon_index = weapon_entity->m_iItemDefinitionIndex();
					
					const EIconType& icon_type = icon_fetcher::get_icon_type_by_weapon_index(weapon_index);
					data.icon = icon_fetcher::get(icon_type);	

					//TODO: set scale accordingly with the distance between the entity and localplayer,
					if (weapon_entity->IsGrenade())
						data.icon.SetScale(0.95f);
					else if (weapon_entity->IsC4())
						data.icon.SetScale(1.1f);
					else data.icon.SetScale(0.55f);
				}
				
				entry_data.dropped_ent_info.push_back(data);
			}
		}

		entity_entry_data.clear();
		entity_entry_data.push_back(std::move(entry_data));
	}
}
