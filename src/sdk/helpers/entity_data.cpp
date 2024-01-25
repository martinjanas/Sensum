#include "entity_data.h"
#include "globals.h"
#include "../helpers/Hitbox_t.h"
#include <algorithm>

class matrix3x4_t
{
public:
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00; m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10; m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20; m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector& vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector const& p)
	{
		m_flMatVal[0][3] = p.x;
		m_flMatVal[1][3] = p.y;
		m_flMatVal[2][3] = p.z;
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				m_flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}

	Vector GetXAxis()  const { return at(0); }
	Vector GetYAxis()  const { return at(1); }
	Vector GetZAxis()  const { return at(2); }
	Vector GetOrigin() const { return at(3); }

	Vector at(int i) const { return Vector{ m_flMatVal[0][i], m_flMatVal[1][i], m_flMatVal[2][i] }; }

	float* operator[](int i) { return m_flMatVal[i]; }
	const float* operator[](int i) const { return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	out.x = in1.dot_product(in2.GetXAxis()) + in2[0][3];
	out.y = in1.dot_product(in2.GetYAxis()) + in2[1][3];
	out.z = in1.dot_product(in2.GetZAxis()) + in2[2][3];
}

//void vector2angles(const Vector& forward, QAngle& angles)
//{
//	float yaw, pitch;
//
//	if (forward.y == 0 && forward.x == 0) 
//	{
//		yaw = 0;
//		if (forward.z > 0)
//			pitch = 270;
//		else
//			pitch = 90;
//	}
//	else {
//		yaw = (atan2(forward.y, forward.x) * 180 / 3.141592654f);
//		if (yaw < 0)
//			yaw += 360;
//
//		const auto tmp = sqrt(forward.x * forward.x + forward.y * forward.y);
//		pitch = (atan2(-forward.z, tmp) * 180 / 3.141592654f);
//		if (pitch < 0)
//			pitch += 360;
//	}
//
//	angles.pitch = pitch;
//	angles.yaw = yaw;
//	angles.roll = 0;
//}

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
	bool GetBBox(CCSPlayerPawn* pawn, BBox_t& bbox) //THIS NEEDS FIXING
	{
		Vector mins, maxs;

		pawn->ComputeSurroundingBox(&mins, &maxs);

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

		float left = std::numeric_limits<float>::max();
		float top = std::numeric_limits<float>::max();
		float right = std::numeric_limits<float>::lowest();
		float bottom = std::numeric_limits<float>::lowest();

		Vector screen_points[8] = { };
		for (int i = 0; i < 8; i++) 
		{
			if (!globals::world2screen(points[i], screen_points[i]))
				return false;

			left = std::min(left, screen_points[i].x);
			top = std::min(top, screen_points[i].y);
			right = std::max(right, screen_points[i].x);
			bottom = std::max(bottom, screen_points[i].y);
		}

		bbox.top = top;
		bbox.left = left;
		bbox.right = right;
		bbox.bottom = bottom;

		return true;
	}

	void hitbox(Vector eye_pos, CCSPlayerPawn* pawn, QAngle& out)
	{
		auto base_animating = pawn->GetBaseAnimating();
		if (!base_animating)
			return;

		auto hitbox_set = base_animating->GetHitboxSet(0);

		if (!hitbox_set)
			return;

		Hitbox_t* hitbox = &hitbox_set->m_HitBoxes()[0];

		if (!hitbox)
			return;

		Vector min = hitbox->m_vMinBounds();
		Vector max = hitbox->m_vMaxBounds();

		Vector hitbox_center = (min + max) * 0.5f;
		
		Vector hitbox_pos = (pawn->GetEyePos() + hitbox_center);
		
		vector2angles(hitbox_pos - eye_pos, out);
		out.ClampNormalize(); //broken

		printf("out: %.1f, %.1f\n", out.pitch, out.yaw);
	}

	void fetch_player_data()
	{
		if (!g::engine_client->IsInGame())
			return;

		std::lock_guard<std::mutex> lock(locker);

		entry_data_t entry_data;
	
		CCSPlayerController* localplayer = nullptr;

		matrix3x4_t bonematrix;

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

			GetBBox(pawn, player_data.bbox);

			QAngle angles;
			g::client->GetViewAngles(0, &angles);

			//printf("pitch: %.1f, yaw: %.1f, roll: %.1f\n", angles.pitch, angles.yaw, angles.roll);

			hitbox(eye_pos, pawn, player_data.aimpos);

			entry_data.player_data.push_back(std::move(player_data));
		}

		player_entry_data.clear();
		player_entry_data.push_front(std::move(entry_data));
	}
}