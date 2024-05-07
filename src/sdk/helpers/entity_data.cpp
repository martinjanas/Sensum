#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"

#include "../localplayer.h"

CCSPlayerController* GetLocalPlayerController()
{
	for (int i = 1; i < 65; ++i)
	{
		CCSPlayerController* player = reinterpret_cast<CCSPlayerController*>(g::entity_system->GetBaseEntity(i));

		if (!player || !player->IsController())
			continue;

		if (player->m_bIsLocalPlayerController())
			return player;
	}
	return nullptr;
}

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
		locker.lock();
		{
			player_instances.clear();
		}
		locker.unlock();
	}

	//https://github.com/nezu-cc/BakaWare4/blob/f82a60479287926b9fa105ea053851da9a7d040e/cheat/src/valve/cs/entity.cpp
	bool GetBBox(CGameSceneNode* scene_node, CCollisionProperty* collision, ABBox_t& out)
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

	bool GetBBoxByHitbox(CCSPlayerPawn* pawn, ABBox_t& out)
	{
		auto hitbox_set = pawn->GetHitboxSet(0);

		if (!hitbox_set)
			return false;

		const auto& hitboxes = hitbox_set->m_HitBoxes();

		Transform_t hitbox_trans[64];

		//TODO: https://github.com/alza54/opensource2/blob/ff7c27a072f059597277b2eeacaf012683d4ff74/OpenSource2-SDK/src/sdk/source-sdk/classes/entity/c_baseentity.cpp#L97
	}

	void get_head_bbox(CCSPlayerPawn* pawn, CCSPlayerPawn* localplayer_pawn, BBox_t& out)
	{
		if (pawn == localplayer_pawn)
			return;

		HitboxSet_t* hitbox_set = pawn->GetHitboxSet(0);

		if (!hitbox_set)
			return;

		Hitbox_t* hitbox = &hitbox_set->m_HitBoxes()[0];
	
		auto skeleton_instance = pawn->m_pGameSceneNode()->GetSkeletonInstance();

		if (!skeleton_instance)
			return;

		auto head_bone = skeleton_instance->m_modelState().bones[6].position;

		Vector mins = (head_bone + hitbox->m_vMinBounds());
		Vector maxs = (head_bone + hitbox->m_vMaxBounds());

		Vector points[8] =
		{
			Vector(mins.x, mins.y, mins.z),
			Vector(mins.x, maxs.y, mins.z),
			Vector(maxs.x, maxs.y, mins.z),
			Vector(maxs.x, mins.y, mins.z),
			Vector(maxs.x, maxs.y, maxs.z),
			Vector(mins.x, maxs.y, maxs.z),
			Vector(mins.x, mins.y, maxs.z),
			Vector(maxs.x, mins.y, maxs.z)
		};

		float left = (std::numeric_limits<float>::max)();
		float top = (std::numeric_limits<float>::max)();
		float right = std::numeric_limits<float>::lowest();
		float bottom = std::numeric_limits<float>::lowest();

		Vector screen_points[8] = { };
		for (int i = 0; i < 8; i++)
		{
			if (!globals::world2screen(points[i], screen_points[i]))
				return;

			left = std::min<float>(left, screen_points[i].x);
			top = std::min<float>(top, screen_points[i].y);
			right = std::max<float>(right, screen_points[i].x);
			bottom = std::max<float>(bottom, screen_points[i].y);
		}

		out.top = top;
		out.left = left;
		out.right = right;
		out.bottom = bottom;
	}

	void hitbox(CCSPlayerPawn* pawn, CCSPlayerPawn* localplayer_pawn, entity_data::player_data_t& data)
	{
		if (pawn == localplayer_pawn)
			return;

		HitboxSet_t* hitbox_set = pawn->GetHitboxSet(0);
		if (!hitbox_set)
			return;

		Transform_t hitbox_trans[HITBOX_MAX];
		int hitbox_count = pawn->HitboxToWorldTransform(hitbox_set, hitbox_trans);
		if (!hitbox_count)
			return;
		
		auto& hitboxes = hitbox_set->m_HitBoxes(); //broken, now indexes from 0 to HITBOX_MAX but something is broken related to hitbox_pos
		if (hitboxes.m_Size == 0)
			return;

		for (int i = 0; i < HITBOX_MAX; i++)
		{
			Hitbox_t hitbox = *hitboxes.AtPtr(i);

			const auto& radius = hitbox.m_flShapeRadius();

			Vector& mins = hitbox.m_vMinBounds();
			Vector& maxs = hitbox.m_vMaxBounds();

			Vector center = (mins + maxs) * 0.5f;

			Vector hitbox_pos = (center + hitbox_trans->m_pos);

			data.hitboxes[i] = { hitbox_pos, hitbox.m_nHitBoxIndex() };
		}
	}

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(locker);

		entry_data_t entry_data;
		for (const auto& instance : player_instances) //TODO: Refactor this in the future
		{
			auto entity = instance.entity;

			if (!entity || !instance.handle.IsValid())
				continue;

			auto controller = reinterpret_cast<CCSPlayerController*>(entity);

			if (!controller)
				continue;

			auto localplayer = GetLocalPlayerController();

			if (!localplayer)
				continue;

			uint32_t index = instance.handle.GetEntryIndex();

			auto pawn = controller->m_hPlayerPawn().Get<CCSPlayerPawn>();

			if (!pawn)
				continue;
			
			auto localplayer_pawn = localplayer->m_hPlayerPawn().Get<CCSPlayerPawn>();

			if (!localplayer_pawn)
				continue;

			if (pawn == localplayer_pawn || pawn->m_pEntity() == localplayer_pawn->m_pEntity())
				continue;

			if (pawn->m_iHealth() <= 0 || !(pawn->m_lifeState() == LIFE_ALIVE))
				continue;

			auto scene_node = pawn->m_pGameSceneNode();

			if (!scene_node)
				continue;

			auto weapon_services = pawn->m_pWeaponServices();

			if (!weapon_services)
				continue;

			auto active_wpn = weapon_services->m_hActiveWeapon().Get<CBasePlayerWeapon>();

			if (!active_wpn)
				continue;

			auto collision = pawn->m_pCollision();

			if (!collision)
				continue;

			auto skeleton_instance = scene_node->GetSkeletonInstance();

			if (!skeleton_instance)
				continue;

			auto& model_state = skeleton_instance->m_modelState();

			auto model = model_state.modelHandle;

			auto eye_pos = localplayer_pawn->GetEyePos();

			player_data_t player_data;
			player_data.player_name = controller->m_sSanitizedPlayerName();
			player_data.index = index;
			player_data.m_vecOrigin = scene_node->m_vecOrigin();
			player_data.m_iHealth = pawn->m_iHealth();
			player_data.m_iShotsFired = pawn->m_iShotsFired();
			player_data.clip = active_wpn->m_iClip1();
			player_data.model_state = &model_state;
			player_data.model = model;
			player_data.pawn = pawn;
			player_data.localplayer_pawn = localplayer_pawn;

			players::localplayer = localplayer_pawn;

			//get_head_bbox(pawn, localplayer_pawn, player_data.head_bbox);

			GetBBox(scene_node, collision, player_data.abbox);

			hitbox(pawn, localplayer_pawn, player_data);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_front(std::move(entry_data));
	}
}