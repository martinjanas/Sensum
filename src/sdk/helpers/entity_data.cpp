#include "entity_data.h"
#include "globals.h"

namespace entity_data
{
	std::list<instance_t> player_instances;
	std::list<instance_t> entity_instances;
	std::list<entry_data_t> player_entry_data;

	std::mutex locker;

	void destroy() //TODO: Call this if localplayer is nullptr
	{
		locker.lock();
		{
			player_instances.clear();
		}
		locker.unlock();
	}

	//https://github.com/nezu-cc/BakaWare4/blob/f82a60479287926b9fa105ea053851da9a7d040e/cheat/src/valve/cs/entity.cpp
	/*bool cs::base_entity::get_bounding_box(bbox& out, bool hitbox) noexcept {
		collision_property* collision = m_pCollision();
		if (!collision)
			return false;

		game_scene_node* scene_node = m_pGameSceneNode();
		if (!scene_node)
			return false;

		const se::transform node_to_world = scene_node->m_nodeToWorld();
		mat3x4 trans;
		node_to_world.to_matrix(trans);

		vec3 min, max;
		if (hitbox) {
			if (!collision->outer)
				return false;

			cs::base_animating* base_animating = collision->outer->get_base_animating();
			if (!base_animating)
				return false;

			se::hitbox_set* hb_set = base_animating->get_hitbox_set(0);
			if (!hb_set || hb_set->hit_boxes.size == 0)
				return false;

			ASSERT(hb_set->hit_boxes.size == 1);

			auto& hitbox = hb_set->hit_boxes[0];
			min = hitbox.mins;
			max = hitbox.maxs;
		}
		else {
			min = collision->m_vecMins();
			max = collision->m_vecMaxs();
		}

		out.x = out.y = std::numeric_limits<float>::max();
		out.w = out.h = -std::numeric_limits<float>::max();

		for (size_t i = 0; i < 8; ++i) {
			const vec3 point = vec3(
				i & 1 ? max.x : min.x,
				i & 2 ? max.y : min.y,
				i & 4 ? max.z : min.z
			).transform(trans);

			vec2 screen;
			if (!math::world_to_screen(point, screen))
				return false;

			out.x = std::floor(std::min(out.x, screen.x));
			out.y = std::floor(std::min(out.y, screen.y));
			out.w = std::floor(std::max(out.w, screen.x));
			out.h = std::floor(std::max(out.h, screen.y));
		}

		const float width = out.w - out.x;
		if (width < 4.f) {
			const float half = (4.f - width) * 0.5f;
			out.x -= std::floor(half);
			out.w += std::ceil(half);
		}

		const float height = out.h - out.y;
		if (height < 4.f) {
			const float half = (4.f - height) * 0.5f;
			out.y -= std::floor(half);
			out.h += std::ceil(half);
		}

		return true;
	}*/

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(locker);

		entry_data_t entry_data;
	
		CCSPlayerController* localplayer = nullptr;

		for (const auto& instance : player_instances)
		{
			auto controller = instance.entity;

			if (!controller || !instance.handle.IsValid())
				continue;

			if (controller->m_bIsLocalPlayerController())
				localplayer = reinterpret_cast<CCSPlayerController*>(controller);

			uint32_t index = instance.handle.GetEntryIndex();

			auto pawn = controller->m_hPawn().Get<CCSPlayerPawnBase>();

			if (!pawn)
				continue;
			
			auto localplayer_pawn = localplayer->m_hPawn().Get();

			if (!localplayer_pawn)
				continue;

			if (pawn == localplayer_pawn || pawn->m_pEntity() == localplayer_pawn->m_pEntity())
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

			auto skeleton_instance = scene_node->GetSkeletonInstance();

			if (!skeleton_instance)
				continue;

			auto& model_state = skeleton_instance->m_modelState();

			auto model = model_state.modelHandle;

			if (pawn->m_iHealth() <= 0 || !(pawn->m_lifeState() == LIFE_ALIVE))
				continue;

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

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.push_front(std::move(entry_data));
	}
}

//This here for now:
//class Matrix4x4
//{
//public:
//	std::array<std::array<float, 4>, 4> data;
//
//	Matrix4x4() {
//		// Initialize the matrix as an identity matrix
//		for (int i = 0; i < 4; ++i) {
//			for (int j = 0; j < 4; ++j) {
//				data[i][j] = (i == j) ? 1.0f : 0.0f;
//			}
//		}
//	}
//
//	// Multiply this matrix by a translation matrix
//	void translate(float x, float y, float z) {
//		data[3][0] += x;
//		data[3][1] += y;
//		data[3][2] += z;
//	}
//
//	// Multiply this matrix by a scale matrix
//	void scale(float x, float y, float z) {
//		for (int i = 0; i < 3; ++i) {
//			data[i][0] = x;
//			data[i][1] = y;
//			data[i][2] *= z;
//		}
//	}
//
//	Vector GetTranslation() const
//	{
//		return { data[3][0], data[3][1], data[3][2] };
//	}
//};
//
//std::array<Matrix4x4, 8> convertToMatrices(Vector& vecMins, Vector& vecMaxs)
//{
//	std::array<Matrix4x4, 8> matrices;
//
//	// Calculate the dimensions of the box
//	Vector dimensions = vecMaxs + vecMins;
//
//	// Create transformation matrices for translation to each corner
//	for (int i = 0; i < 8; ++i)
//	{
//		float tx = (i & 1) ? vecMaxs.x : vecMins.x;
//		float ty = (i & 2) ? vecMaxs.y : vecMins.y;
//		float tz = (i & 4) ? vecMaxs.z : vecMins.z;
//
//		matrices[i].translate(tx, ty, tz);
//	}
//
//	return matrices;
//};
