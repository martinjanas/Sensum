#include "entity_data.h"
#include "globals.h"

#include "../helpers/Hitbox_t.h"

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

	std::vector<Vector> matrixToVector(const Vector& min, const Vector& max) {
		std::vector<Vector> points;

		points.push_back(Vector(min.x, min.y, min.z));
		points.push_back(Vector(min.x, max.y, min.z));
		points.push_back(Vector(max.x, max.y, min.z));
		points.push_back(Vector(max.x, min.y, min.z));
		points.push_back(Vector(max.x, max.y, max.z));
		points.push_back(Vector(min.x, max.y, max.z));
		points.push_back(Vector(min.x, min.y, max.z));
		points.push_back(Vector(max.x, min.y, max.z));

		return points;
	}

	//https://github.com/nezu-cc/BakaWare4/blob/f82a60479287926b9fa105ea053851da9a7d040e/cheat/src/valve/cs/entity.cpp
	bool GetBBox(CCSPlayerPawn* pawn, BBox_t& bbox) //THIS NEEDS FIXING
	{
		auto collision = pawn->m_pCollision();
		if (!collision)
			return false;

		Vector vecMax = pawn->m_pGameSceneNode()->m_vecAbsOrigin() + collision->m_vecMaxs();
		Vector vecMin = pawn->m_pGameSceneNode()->m_vecAbsOrigin() + collision->m_vecMins();

		Vector arrPoints[8] =
		{
			Vector(vecMin.x, vecMin.y, vecMin.z),
			Vector(vecMin.x, vecMax.y, vecMin.z),
			Vector(vecMax.x, vecMax.y, vecMin.z),
			Vector(vecMax.x, vecMin.y, vecMin.z),
			Vector(vecMax.x, vecMax.y, vecMax.z),
			Vector(vecMin.x, vecMax.y, vecMax.z),
			Vector(vecMin.x, vecMin.y, vecMax.z),
			Vector(vecMax.x, vecMin.y, vecMax.z)
		};

		float flLeft = std::numeric_limits<float>::max();
		float flTop = std::numeric_limits<float>::max();
		float flRight = std::numeric_limits<float>::lowest();
		float flBottom = std::numeric_limits<float>::lowest();

		// get screen points position
		Vector arrScreen[8] = { };
		for (std::size_t i = 0U; i < 8U; i++) {
			if (!globals::world_to_screen(arrPoints[i], arrScreen[i]))
				return false;

			flLeft = std::min(flLeft, arrScreen[i].x);
			flTop = std::min(flTop, arrScreen[i].y);
			flRight = std::max(flRight, arrScreen[i].x);
			flBottom = std::max(flBottom, arrScreen[i].y);
		}


		bbox.top = flTop;
		bbox.left = flLeft;
		bbox.right = flRight;
		bbox.bottom = flBottom;
		return true;
	}

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(locker);

		entry_data_t entry_data;
	
		CCSPlayerController* localplayer = nullptr;

		for (const auto& instance : player_instances)
		{
			auto entity = instance.entity;

			if (!entity || !instance.handle.IsValid())
				continue;

			auto controller = reinterpret_cast<CCSPlayerController*>(entity);

			if (!controller)
				continue;

			if (controller->m_bIsLocalPlayerController())
				localplayer = reinterpret_cast<CCSPlayerController*>(controller);

			uint32_t index = instance.handle.GetEntryIndex();

			auto pawn = controller->m_hPawn().Get<CCSPlayerPawn>();

			if (!pawn)
				continue;
			
			auto localplayer_pawn = localplayer->m_hPawn().Get<CCSPlayerPawn>();

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

			GetBBox(pawn, player_data.bbox);

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
