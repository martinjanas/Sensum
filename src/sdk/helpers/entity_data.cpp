#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"

#include "../localplayer.h"

#define MASK_PLAYER_VISIBILITY (CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_HITBOX | CONTENTS_GRATE)

namespace entity_data
{
	std::list<instance_t> player_instances;
	std::list<instance_t> entity_instances;
	std::list<entry_data_t> player_entry_data;

	std::mutex locker;

	Vector origin;

	namespace view_matrix
	{
		VMatrix* matrix;
	};

	void destroy()
	{
		std::lock_guard<std::mutex> lock(locker, std::adopt_lock);

		player_entry_data.clear();
	}

	bool GetBBox(CGameSceneNode* scene_node, CCollisionProperty* collision, BBox_t& out)
	{
		Vector mins = collision->m_vecMins();
		Vector maxs = collision->m_vecMaxs();

		const matrix3x4_t& transform = scene_node->m_nodeToWorld().ToMatrix3x4();

		bool valid = true;
		for (int i = 0; i < 8; i++)
		{
			Vector point = Vector{ i & 1 ? maxs.x : mins.x, i & 2 ? maxs.y : mins.y, i & 4 ? maxs.z : mins.z };
			point = point.transform(transform);

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
			case HITBOX_RIGHT_HAND:
			case HITBOX_LEFT_HAND:
			case HITBOX_LEFT_FOREARM:
			case HITBOX_RIGHT_FOREARM:
				return true;
			default:
				return false;	 		
		}
	}

	void GetHitbox(entity_data::player_data_t& player_data, const Vector& eye_pos, CCSPlayerPawn* local_pawn, const bool& on_screen)
	{
		HitboxSet_t* hitbox_set = player_data.m_PlayerPawn->GetHitboxSet(0);
		if (!hitbox_set)
			return;

		auto& hitboxes = hitbox_set->m_HitBoxes();
		if (hitboxes.Count() == 0 || hitboxes.Count() > HITBOX_MAX)
			return;
		//MASK_PLAYER_VISIBILITY
		//TraceFilter_t filter(CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX, local_pawn, player_data.m_PlayerPawn, 4); //4 //0x1C3003
		//TraceFilter_t filter(0x1C3003, local_pawn, nullptr, 4); //4 //0x1C3003

		Ray_t ray;
		//TraceFilter_t filter(CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_DEBRIS | CONTENTS_HITBOX, nullptr, nullptr, 0); //totally fucking broken, the trace just goes thru walls no matter the mask

		TraceFilter_t filter(MASK_VISIBLE, player_data.m_PlayerPawn, 4);
		
		for (int i = 0; i < HITBOX_MAX; ++i)
		{
			Hitbox_t* hitbox = &hitboxes[i];

			if (!hitbox)
				continue;
				
			if (!player_data.m_PlayerPawn->HitboxToWorldTransform(hitbox_set, player_data.hitbox_transform))
				continue;

			const auto& hitbox_matrix = player_data.hitbox_transform[i];

			const auto& radius = hitbox->m_flShapeRadius() != -1 ? hitbox->m_flShapeRadius() : 0.f;
	
			auto min_bounds = hitbox->m_vMinBounds() - radius;
			auto max_bounds = hitbox->m_vMaxBounds() + radius;

			const auto& mins = min_bounds.transform(hitbox_matrix.ToMatrix3x4());
			const auto& maxs = max_bounds.transform(hitbox_matrix.ToMatrix3x4());

			auto hitbox_pos = (mins + maxs) * 0.5f;

			player_data.hitboxes[i] = { hitbox_pos, hitbox->m_nHitBoxIndex(), player_data.m_szPlayerName };

			if (!on_screen || !hitbox_visible_set(i))
				continue;

			if (!player_data.is_visible) //TODO: Not working correctly - this is fucking fucked, the ray goes thru walls and shit
			{
				auto forward = player_data.m_vecEyeAngles.to_vector();
				forward *= 512.f;

				auto start = player_data.m_vecEyePos;
				auto end = start + forward;

				//ray.start = start;
				//ray.end = end;

				g::game_trace->TraceShape(&ray, start, end, &filter, &player_data.trace);
				
				//g::game_trace->ClipRayToEntity(&ray, eye_pos, hitbox_pos, player_data.m_PlayerPawn, &filter, &player_data.trace);
	
				player_data.is_visible = true;//player_data.trace.m_vecEndPos.dist_to(eye_pos) == hitbox_pos.dist_to(eye_pos);
			}
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

			const auto on_screen = globals::world2screen(scene_node->m_vecOrigin(), origin);

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
			
			player_data.is_visible = false;

			GetHitbox(player_data, eye_pos, localpawn, on_screen);
			GetBBox(scene_node, collision, player_data.bbox);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_front(std::move(entry_data));
	}
}