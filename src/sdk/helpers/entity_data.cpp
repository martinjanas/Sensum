#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"

#include "../localplayer.h"

namespace entity_data
{
	std::list<instance_t> player_instances;
	std::list<instance_t> entity_instances;
	std::list<entry_data_t> player_entry_data;

	std::mutex locker;

	namespace view_matrix
	{
		VMatrix* matrix;
	};

	void destroy() //TODO: Call this if localplayer is nullptr
	{
		std::lock_guard<std::mutex> lock(entity_data::locker, std::adopt_lock);

		player_instances.clear();
	}

	//https://github.com/nezu-cc/BakaWare4/blob/f82a60479287926b9fa105ea053851da9a7d040e/cheat/src/valve/cs/entity.cpp
	bool GetBBox(CGameSceneNode* scene_node, CCollisionProperty* collision, BBox_t& out)
	{
		Vector mins = collision->m_vecMins();
		Vector maxs = collision->m_vecMaxs();
		
		//printf("mins: %s, maxs: %s\n", mins.ToString().c_str(), maxs.ToString().c_str());
		//mins: -16, -16, 0
		//maxs:  16, 16, 72 (54 crouching)

		out.Invalidate();

		for (int i = 0; i < 8; ++i) 
		{
			Vector worldPoint = Vector{ i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z };

			if (!globals::world2screen(worldPoint.transform(scene_node->m_nodeToWorld().ToMatrix3x4()), out.m_Vertices[i]))
				return false;

			out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
			out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
		}

		return true;
	}

	void GetHitbox(entity_data::player_data_t& player_data)
	{
		HitboxSet_t* hitbox_set = player_data.m_PlayerPawn->GetHitboxSet(0);
		if (!hitbox_set)
			return;

		auto& hitboxes = hitbox_set->m_HitBoxes();
		if (hitboxes.Count() == 0 || hitboxes.Count() > HITBOX_MAX)
			return;

		for (int i = 0; i < HITBOX_MAX; ++i)
		{
			Hitbox_t* hitbox = &hitboxes[i];

			if (!hitbox)
				continue;

			if (!player_data.m_PlayerPawn->HitboxToWorldTransform(hitbox_set, player_data.hitbox_transform))
				continue;

			const auto& hitbox_matrix = player_data.hitbox_transform[i];

			const auto& radius = hitbox->m_flShapeRadius() != -1 ? hitbox->m_flShapeRadius() : 0.f;
	
			//This code below extends the hitbox bounds, because we are modifying the m_vMin & m_vMax properties:
			/*const auto& mins = (hitbox->m_vMinBounds() - radius).transform(hitbox_matrix.ToMatrix3x4());
			const auto& maxs = (hitbox->m_vMaxBounds() + radius).transform(hitbox_matrix.ToMatrix3x4());*/

			auto min_bounds = hitbox->m_vMinBounds() - radius;
			auto max_bounds = hitbox->m_vMaxBounds() + radius;

			
			const auto& mins = min_bounds.transform(hitbox_matrix.ToMatrix3x4());
			const auto& maxs = max_bounds.transform(hitbox_matrix.ToMatrix3x4());

			auto hitbox_pos = (mins + maxs) * 0.5f;

			player_data.hitboxes[i] = { hitbox_pos, hitbox->m_nHitBoxIndex(), player_data.m_szPlayerName };
		}
	}

	void fetch_player_data(CUserCmd* cmd)
	{
		if (!g::engine_client->IsInGame())
			return;
		
		const auto& localplayer = g::entity_system->GetLocalPlayerController<CCSPlayerController*>();
		if (!localplayer)
		{
			//destroy();

			return;
		}

		const auto& localpawn = localplayer->m_hPlayerPawn().Get<CCSPlayerPawn*>();
		if (!localpawn || !localpawn->IsAlive())
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

			const uint32_t& index = instance.handle.GetIndex();
			if (index <= 0 || index > 0x7FFF)
				continue;

			const auto& pawn = controller->m_hPlayerPawn().Get<CCSPlayerPawn*>();
			if (!pawn || !pawn->IsAlive() || pawn == localpawn || pawn->m_iTeamNum() == local_team)
				continue;

			auto hitbox_set = pawn->GetHitboxSet(0);
			if (!hitbox_set)
				continue;

			const auto scene_node = pawn->m_pGameSceneNode();
			const auto weapon_services = pawn->m_pWeaponServices();
			if (!scene_node || !weapon_services)
				continue;

			const auto active_wpn = weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon*>();
			const auto collision = pawn->m_pCollision();
			const auto skeleton_instance = scene_node->GetSkeletonInstance();
			if (!active_wpn || !collision || !skeleton_instance)
				continue;

			const auto model_state = skeleton_instance->m_modelState();
			const auto model = model_state.modelHandle;
			if (!model.IsValid())
				continue;

			player_data_t player_data;
			player_data.m_szPlayerName = controller->m_sSanitizedPlayerName();
			player_data.m_iPlayerIndex = index;
			player_data.m_vecOrigin = scene_node->m_vecOrigin();
			player_data.m_iHealth = pawn->m_iHealth();
			player_data.m_iShotsFired = pawn->m_iShotsFired();
			player_data.m_iClip1 = active_wpn->m_iClip1();
			player_data.m_ModelState = model_state;
			player_data.m_hModel = model;
			player_data.m_PlayerPawn = pawn;
			
			GetHitbox(player_data);
			GetBBox(scene_node, collision, player_data.bbox);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_front(std::move(entry_data));
	}
}