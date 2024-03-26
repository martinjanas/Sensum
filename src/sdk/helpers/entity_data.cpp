#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include <DirectXMath.h>
#include "../../thirdparty/ImGui/imgui_internal.h"

#define DEG2RAD(x) ((x) * static_cast<float>(3.14159265358979323846) / 180.0f)

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

void vector2angles(const Vector& forward, QAngle& angles)
{
	float yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0) {
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else {
		yaw = (atan2(forward[1], forward[0]) * 180 / 3.141592654f);
		if (yaw < 0)
			yaw += 360;

		const auto tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / 3.141592654f);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void angle2vectors(const QAngle& angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
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

			if (!globals::world2screen(worldPoint.Transform(scene_node->m_nodeToWorld().ToMatrix3x4()), out.m_Vertices[i]))
				return false;

			out.m_Mins = ImMin(out.m_Mins, out.m_Vertices[i]);
			out.m_Maxs = ImMax(out.m_Maxs, out.m_Vertices[i]);
		}

		return true;
	}

	bool GetBBoxByHitbox(CCSPlayerPawn* pawn, ABBox_t& out)
	{
		auto base_animating = pawn->GetBaseAnimating();

		if (!base_animating)
			return false;

		auto hitbox_set = base_animating->GetHitboxSet(0);

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

		auto base_animating = pawn->GetBaseAnimating();
		if (!base_animating)
			return;

		HitboxSet_t* hitbox_set = base_animating->GetHitboxSet(0);

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

	void hitbox(Vector eye_pos, CCSPlayerPawn* pawn, CCSPlayerPawn* localplayer_pawn, QAngle& out)
	{
		if (pawn == localplayer_pawn)
			return;

		auto base_animating = pawn->GetBaseAnimating();
		if (!base_animating)
			return;

		HitboxSet_t* hitbox_set = base_animating->GetHitboxSet(0);

		if (!hitbox_set)
			return;

		Transform_t hitbox_trans[65];

		pawn->HitboxToWorldTransform(hitbox_set, hitbox_trans);
		
		Hitbox_t* hitbox = &hitbox_set->m_HitBoxes()[0];

		if (!hitbox)
			return;

		if (hitbox->m_nHitBoxIndex() == 0)
		{
			Vector mins = hitbox->m_vMinBounds();
			Vector maxs = hitbox->m_vMaxBounds();

			Vector center = (mins + maxs) * 0.5f;

			Vector hitbox_pos = (hitbox_trans->m_pos - center);
			vector2angles(hitbox_pos - eye_pos, out);
			out.ClampNormalize();

			printf("hitbox_pos: %s, pawn_eyepos: %s\n", hitbox_pos.ToString().c_str(), pawn->GetEyePos().ToString().c_str());
		}
	}

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		if (player_instances.empty())
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
			player_data.m_iHealth = pawn->m_iHealth();
			player_data.m_iShotsFired = pawn->m_iShotsFired();
			player_data.m_vecOrigin = scene_node->m_vecOrigin();
			player_data.m_vOldOrigin = pawn->m_vOldOrigin();
			player_data.clip = active_wpn->m_iClip1();
			player_data.model_state = &model_state;
			player_data.model = model;
			player_data.index = index;
			player_data.pawn = pawn;

			//get_head_bbox(pawn, localplayer_pawn, player_data.head_bbox);

			GetBBox(scene_node, collision, player_data.abbox);

			hitbox(eye_pos, pawn, localplayer_pawn, player_data.aimpos);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_front(std::move(entry_data));
	}
}