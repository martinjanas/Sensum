#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"
#include "../localplayer.h"

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
		std::lock_guard<std::mutex> lock(locker, std::adopt_lock);

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

	bool hitbox_visible_set(int index)
	{
		switch (index) 
		{
			case HITBOX_HEAD:
			case HITBOX_LOWER_CHEST:
			case HITBOX_UPPER_CHEST:
			case HITBOX_RIGHT_CALF:
			case HITBOX_LEFT_CALF:
			//case HITBOX_RIGHT_HAND:
			//case HITBOX_LEFT_HAND:
			case HITBOX_LEFT_FOREARM:
			case HITBOX_RIGHT_FOREARM:
				return true;
			default:
				return false;	 		
		}
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

	void update_visiblity(entity_data::hitbox_info_t& hitbox_info, entity_data::player_data_t& player_data, CCSPlayerPawn* local_pawn)
	{
		if (!hitbox_info.visible)
		{
			Ray_t ray = { };
			TraceFilter_t filter(MASK_PLAYER_VISIBLE, local_pawn, nullptr, 4);
			Trace_t trace = { };

			g::engine_trace->TraceShape(&ray, local_pawn->GetEyePos(), hitbox_info.hitbox_pos, &filter, &trace);

			if (!player_data.flags.test(PLAYER_VISIBLE) && (trace.m_pHitEntity == player_data.m_PlayerPawn || trace.m_flFraction >= 0.97f))
				player_data.flags.set(PLAYER_VISIBLE);

			(trace.m_pHitEntity == player_data.m_PlayerPawn || trace.m_flFraction >= 0.97f) ? hitbox_info.visible = true : hitbox_info.visible = false;
		}

		if (player_data.flags.test(PLAYER_VISIBLE) && !player_data.flags.test(PLAYER_IN_SMOKE) && is_in_smoke(local_pawn->GetEyePos(), hitbox_info.hitbox_pos, 0.2f))
			player_data.flags.set(PLAYER_IN_SMOKE);
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

			hitbox_info.hitbox_pos = hitbox_pos;
			hitbox_info.index = hitbox->m_nHitBoxIndex();
			hitbox_info.visible = false;

			if (!on_screen || !hitbox_visible_set(i))
				continue;

			update_visiblity(hitbox_info, player_data, local_pawn);

			player_data.hitboxes[i] = hitbox_info;
		}
	}

	void fetch_player_data(CUserCmd* cmd)
	{
		if (!g::engine_client->IsInGame())
			return;
		
		const auto& local_controller = g::entity_system->GetLocalPlayerController<CCSPlayerController*>(); //players::localplayer;
		if (!local_controller)
		{
			destroy();
			return;
		}

		const auto& localpawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(local_controller->m_hPlayerPawn());
		if (!localpawn)
			return;

		std::lock_guard<std::mutex> lock(locker);

		const auto& local_team = localpawn->m_iTeamNum();
		const auto& eye_pos = localpawn->GetEyePos();
		
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

			const auto& pawn = g::entity_system->GetEntityFromHandle<CCSPlayerPawn*>(controller->m_hPlayerPawn());  //controller->m_hPlayerPawn().Get<CCSPlayerPawn*>();
			if (!pawn || !pawn->IsAlive() || pawn == localpawn || pawn->m_iTeamNum() == local_team)
				continue;

			auto hitbox_set = pawn->GetHitboxSet(0);
			if (!hitbox_set)
				continue;

			const auto scene_node = pawn->m_pGameSceneNode();
			const auto weapon_services = pawn->m_pWeaponServices();
			if (!scene_node || !weapon_services)
				continue;

			const auto active_wpn = g::entity_system->GetEntityFromHandle<CBasePlayerWeapon*>(weapon_services->m_hActiveWeapon()); //weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon*>();
			const auto collision = pawn->m_pCollision();
			const auto skeleton_instance = scene_node->GetSkeletonInstance();
			if (!active_wpn || !collision || !skeleton_instance)
				continue;

			const auto model_state = skeleton_instance->m_modelState();
			const auto model = model_state.modelHandle;
			if (!model.IsValid())
				continue;

			Vector origin_w2s;
			const auto on_screen = globals::world2screen(scene_node->m_vecOrigin(), origin_w2s);

			player_data_t player_data;
			player_data.m_szPlayerName = "Martin";//controller->m_sSanitizedPlayerName();
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

			player_data.flags.reset(PLAYER_VISIBLE);
			player_data.flags.reset(PLAYER_IN_SMOKE);

			pawn->m_iHealth() > 0 ? player_data.flags.set(PLAYER_ALIVE) : player_data.flags.reset(PLAYER_ALIVE);
			pawn->InAir() ? player_data.flags.set(PLAYER_IN_AIR) : player_data.flags.reset(PLAYER_IN_AIR);

			get_hitboxes(player_data, eye_pos, localpawn, on_screen);
			get_bbox(scene_node, collision, player_data.bbox);
			get_bones_w2s(player_data);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_back(std::move(entry_data));
	}
}