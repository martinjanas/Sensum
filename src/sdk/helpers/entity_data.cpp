#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"
#include "../localplayer.h"
#include "../../features/features.h"
#include <queue>

namespace entity_data
{
	std::list<EntityInstance_t> player_instances;
	std::list<EntityInstance_t> entity_instances;
	std::list<entry_data_t> player_entry_data;

	std::mutex locker;

	namespace view_matrix
	{
		VMatrix* matrix;
	};

	void destroy()
	{
		player_entry_data.clear();
	}

	bool is_dist_equal(float dist1, float dist2) 
	{
		float epsilon = std::numeric_limits<float>::epsilon() * std::max(std::fabs(dist1), std::fabs(dist2));
		return std::fabs(dist1 - dist2) < epsilon;
	}

	void get_bones_w2s(entity_data::player_data_t& data)
	{
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

	bool get_bbox(CGameSceneNode* scene_node, CCollisionProperty* collision, BBox_t& out)
	{
		Vector mins = collision->m_vecMins();
		Vector maxs = collision->m_vecMaxs();

		const matrix3x4_t& matrix = scene_node->m_nodeToWorld().ToMatrix3x4();

		bool valid = true;
		for (int i = 0; i < 8; i++)
		{
			Vector point = Vector{ i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z }.transform(matrix);

			valid &= globals::world2screen(point, out.m_Vertices[i]);
		}

		if (!valid)
			return false;

		out.m_Mins = out.m_Vertices[0];
		out.m_Maxs = out.m_Vertices[0];
		for (int i = 1; i < 8; i++)
		{
			out.m_Mins.x = std::min(out.m_Mins.x, out.m_Vertices[i].x);
			out.m_Mins.y = std::min(out.m_Mins.y, out.m_Vertices[i].y);
			out.m_Maxs.x = std::max(out.m_Maxs.x, out.m_Vertices[i].x);
			out.m_Maxs.y = std::max(out.m_Maxs.y, out.m_Vertices[i].y);
		}

		return true;
	}

	bool is_in_smoke(const Vector& start, const Vector& end, const float& max_density)
	{
		using fn = float(__fastcall*)(const Vector&, const Vector&, void*);
		static auto addr = modules::client.scan("E8 ? ? ? ? 0F 28 F8 44 0F 28 54 24 ?", "IsInSmoke").add(0x1).abs().as();
		if (!addr)
			return false;

		auto get_smoke_density = reinterpret_cast<fn>(addr);
		if (get_smoke_density)
		{
			float smoke_density = get_smoke_density(start, end, nullptr);

			return smoke_density >= max_density;
		}

		return false;
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

			g::engine_trace->TraceShape(&ray, local_pawn->GetEyePos(), hitbox.hitbox_pos, &filter, &trace);

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

	void fetch_player_data(CUserCmd* cmd)
	{
		if (!g::engine_client->IsInGame())
			return;
	
		std::lock_guard<std::mutex> lock(locker);

		const auto& local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
		if (!local_controller)
		{
			destroy();
			return;
		}

		const auto& localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPlayerPawn());
		if (!localpawn)
			return;

		const auto& local_team = localpawn->m_iTeamNum();
		const auto& eye_pos = localpawn->GetEyePos();
		
		static Vector old_origin;
		static QAngle old_angle;

		entry_data_t entry_data;
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

			const auto& pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(controller->m_hPawn()); //was m_hPlayerPawn
			if (!pawn || !pawn->IsAlive() || pawn->m_bGunGameImmunity() || pawn == localpawn || (!g::game_type->IsDeathmatch() && pawn->m_iTeamNum() == local_team))
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

			const auto& active_wpn = g::entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(active_wpn_handle); //weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon*>();
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
			
			player_data.flags.reset();

			pawn->m_iHealth() > 0 ? player_data.flags.set(PLAYER_ALIVE) : player_data.flags.reset(PLAYER_ALIVE);
			pawn->InAir() ? player_data.flags.set(PLAYER_IN_AIR) : player_data.flags.reset(PLAYER_IN_AIR);

			if (old_origin != scene_node->m_vecOrigin() || (old_angle.pitch != pawn->m_angEyeAngles().yaw || old_angle.pitch != pawn->m_angEyeAngles().yaw))
			{
				get_hitboxes(player_data, eye_pos, localpawn, on_screen);
				get_bbox(scene_node, collision, player_data.bbox);
				get_bones_w2s(player_data);
			}

			old_origin = scene_node->m_vecOrigin();
			old_angle = pawn->m_angEyeAngles();

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_back(std::move(entry_data));
	}
}