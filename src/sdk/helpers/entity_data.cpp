#include "entity_data.h"
#include "../helpers/Hitbox_t.h"
#include "../helpers/globals.h"
#include "../../thirdparty/ImGui/imgui_internal.h"

#include "../localplayer.h"

#define MASK_PLAYER_VISIBILITY (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_HITBOX)
#define MASK_PLAYER_VISIBLE (CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_PLAYER | CONTENTS_NPC | CONTENTS_DEBRIS | CONTENTS_HITBOX | CONTENTS_BLOCK_LOS)


namespace entity_data
{
	std::list<EntityInstance_t> player_instances;
	std::list<EntityInstance_t> entity_instances;
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

	bool is_dist_equal(float dist1, float dist2) 
	{
		float epsilon = std::numeric_limits<float>::epsilon() * std::max(std::fabs(dist1), std::fabs(dist2));
		return std::fabs(dist1 - dist2) < epsilon;
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
			//case HITBOX_RIGHT_HAND:
			//case HITBOX_LEFT_HAND:
			case HITBOX_LEFT_FOREARM:
			case HITBOX_RIGHT_FOREARM:
				return true;
			default:
				return false;	 		
		}
	}

	bool IsInSmoke(const Vector& start, const Vector& end, const float& max_density)
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

	void GetHitbox(entity_data::player_data_t& player_data, const Vector& eye_pos, CCSPlayerPawn* local_pawn, const bool& on_screen)
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
	
			auto min_bounds = hitbox->m_vMinBounds() - radius;
			auto max_bounds = hitbox->m_vMaxBounds() + radius;

			/*auto min_bounds = hitbox->m_vMinBounds();
			auto max_bounds = hitbox->m_vMaxBounds();*/

			const auto& mins = min_bounds.transform(hitbox_matrix.ToMatrix3x4());
			const auto& maxs = max_bounds.transform(hitbox_matrix.ToMatrix3x4());

			auto hitbox_pos = (mins + maxs) * 0.5f;

			player_data.hitboxes[i] = { hitbox_pos, hitbox->m_nHitBoxIndex() };

			if (!on_screen || !hitbox_visible_set(i))
				continue;

			Vector start = local_pawn->GetEyePos();
			Vector end = hitbox_pos;

			if (!player_data.flags.test(PLAYER_VISIBLE))
			{
				Ray_t ray = { };
				TraceFilter_t filter(MASK_PLAYER_VISIBLE, local_pawn, nullptr, 4);
				Trace_t trace = { };

				g::engine_trace->TraceShape(&ray, start, end, &filter, &trace);

				(trace.m_pHitEntity == player_data.m_PlayerPawn || trace.m_flFraction >= 0.97f) ? player_data.flags.set(PLAYER_VISIBLE) : player_data.flags.reset(PLAYER_VISIBLE);
			}

			if (player_data.flags.test(PLAYER_VISIBLE) && !player_data.flags.test(PLAYER_IN_SMOKE) && IsInSmoke(start, end, 0.2f))
				player_data.flags.set(PLAYER_IN_SMOKE);
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
			
			player_data.flags.reset();

			player_data.flags.reset(PLAYER_VISIBLE);
			player_data.flags.reset(PLAYER_IN_SMOKE);

			pawn->m_iHealth() > 0 ? player_data.flags.set(PLAYER_ALIVE) : player_data.flags.reset(PLAYER_ALIVE);
			pawn->InAir() ? player_data.flags.set(PLAYER_IN_AIR) : player_data.flags.reset(PLAYER_IN_AIR);

			GetHitbox(player_data, eye_pos, localpawn, on_screen);
			GetBBox(scene_node, collision, player_data.bbox);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.emplace_back(std::move(entry_data));
	}
}