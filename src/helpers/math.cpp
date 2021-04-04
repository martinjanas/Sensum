#include "math.h"
#include "utils.h"
#include "console.h"
#include "../settings/globals.h"
#include "../valve_sdk/csgostructs.hpp"

#include <d3dx9.h>
#include <D3dx9math.h>
#include <algorithm>

void math::correct_movement(CUserCmd* cmd, const QAngle& old_angles)
{
	correct_movement(cmd, old_angles, cmd->forwardmove, cmd->sidemove);
}

void math::correct_movement(CUserCmd* cmd, const QAngle& old_angles, const float& old_forward, const float& old_sidemove)
{
	// side/forward move correction
	float f1, f2, deltaView;
	if (old_angles.yaw < 0.f)
		f1 = 360.0f + old_angles.yaw;
	else
		f1 = old_angles.yaw;

	if (cmd->viewangles.yaw < 0.0f)
		f2 = 360.0f + cmd->viewangles.yaw;
	else
		f2 = cmd->viewangles.yaw;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);

	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * old_forward + cos(DEG2RAD(deltaView + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * old_forward + sin(DEG2RAD(deltaView + 90.f)) * old_sidemove;
}

void AngleVectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up) {
	float sr, sp, sy, cr, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
	DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

template<class T>
void Normalize3(T& vec) {
	for (auto i = 0; i < 2; i++) {
		while (vec[i] < -180.0f) vec[i] += 360.0f;
		while (vec[i] > 180.0f) vec[i] -= 360.0f;
	}
	vec[2] = 0.f;
}

void ClampAngles(QAngle& angles) {
	if (angles.pitch > 89.0f) angles.pitch = 89.0f;
	else if (angles.pitch < -89.0f) angles.pitch = -89.0f;

	if (angles.yaw > 180.0f) angles.yaw = 180.0f;
	else if (angles.yaw < -180.0f) angles.yaw = -180.0f;

	angles.roll = 0;
}

void math::FixAngles(QAngle& angles) {
	Normalize3(angles);
	ClampAngles(angles);
}

void math::MovementFix(CUserCmd* m_Cmd, QAngle wish_angle, QAngle old_angles) {
	if (old_angles.pitch != wish_angle.pitch || old_angles.yaw != wish_angle.yaw || old_angles.roll != wish_angle.roll) {
		Vector wish_forward, wish_right, wish_up, cmd_forward, cmd_right, cmd_up;

		auto viewangles = old_angles;
		auto movedata = Vector(m_Cmd->forwardmove, m_Cmd->sidemove, m_Cmd->upmove);
		viewangles.Normalize();

		if (!(g::local_player->m_fFlags() & FL_ONGROUND) && viewangles.roll != 0.f)
			movedata.y = 0.f;

		AngleVectors(wish_angle, wish_forward, wish_right, wish_up);
		AngleVectors(viewangles, cmd_forward, cmd_right, cmd_up);

		auto v8 = sqrt(wish_forward.x * wish_forward.x + wish_forward.y * wish_forward.y), v10 = sqrt(wish_right.x * wish_right.x + wish_right.y * wish_right.y), v12 = sqrt(wish_up.z * wish_up.z);

		Vector wish_forward_norm(1.0f / v8 * wish_forward.x, 1.0f / v8 * wish_forward.y, 0.f),
			wish_right_norm(1.0f / v10 * wish_right.x, 1.0f / v10 * wish_right.y, 0.f),
			wish_up_norm(0.f, 0.f, 1.0f / v12 * wish_up.z);

		auto v14 = sqrt(cmd_forward.x * cmd_forward.x + cmd_forward.y * cmd_forward.y), v16 = sqrt(cmd_right.x * cmd_right.x + cmd_right.y * cmd_right.y), v18 = sqrt(cmd_up.z * cmd_up.z);

		Vector cmd_forward_norm(1.0f / v14 * cmd_forward.x, 1.0f / v14 * cmd_forward.y, 1.0f / v14 * 0.0f),
			cmd_right_norm(1.0f / v16 * cmd_right.x, 1.0f / v16 * cmd_right.y, 1.0f / v16 * 0.0f),
			cmd_up_norm(0.f, 0.f, 1.0f / v18 * cmd_up.z);

		auto v22 = wish_forward_norm.x * movedata.x, v26 = wish_forward_norm.y * movedata.x, v28 = wish_forward_norm.z * movedata.x, v24 = wish_right_norm.x * movedata.y, v23 = wish_right_norm.y * movedata.y, v25 = wish_right_norm.z * movedata.y, v30 = wish_up_norm.x * movedata.z, v27 = wish_up_norm.z * movedata.z, v29 = wish_up_norm.y * movedata.z;

		Vector correct_movement;
		correct_movement.x = cmd_forward_norm.x * v24 + cmd_forward_norm.y * v23 + cmd_forward_norm.z * v25 + (cmd_forward_norm.x * v22 + cmd_forward_norm.y * v26 + cmd_forward_norm.z * v28) + (cmd_forward_norm.y * v30 + cmd_forward_norm.x * v29 + cmd_forward_norm.z * v27);
		correct_movement.y = cmd_right_norm.x * v24 + cmd_right_norm.y * v23 + cmd_right_norm.z * v25 + (cmd_right_norm.x * v22 + cmd_right_norm.y * v26 + cmd_right_norm.z * v28) + (cmd_right_norm.x * v29 + cmd_right_norm.y * v30 + cmd_right_norm.z * v27);
		correct_movement.z = cmd_up_norm.x * v23 + cmd_up_norm.y * v24 + cmd_up_norm.z * v25 + (cmd_up_norm.x * v26 + cmd_up_norm.y * v22 + cmd_up_norm.z * v28) + (cmd_up_norm.x * v30 + cmd_up_norm.y * v29 + cmd_up_norm.z * v27);

		correct_movement.x = std::clamp(correct_movement.x, -450.f, 450.f);
		correct_movement.y = std::clamp(correct_movement.y, -450.f, 450.f);
		correct_movement.z = std::clamp(correct_movement.z, -320.f, 320.f);

		m_Cmd->forwardmove = correct_movement.x;
		m_Cmd->sidemove = correct_movement.y;
		m_Cmd->upmove = correct_movement.z;

		m_Cmd->buttons &= ~(IN_MOVERIGHT | IN_MOVELEFT | IN_BACK | IN_FORWARD);
		if (m_Cmd->sidemove != 0.0) {
			if (m_Cmd->sidemove <= 0.0)
				m_Cmd->buttons |= IN_MOVELEFT;
			else
				m_Cmd->buttons |= IN_MOVERIGHT;
		}

		if (m_Cmd->forwardmove != 0.0) {
			if (m_Cmd->forwardmove <= 0.0)
				m_Cmd->buttons |= IN_BACK;
			else
				m_Cmd->buttons |= IN_FORWARD;
		}
	}
}

void math::VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	out[0] = in1.Dot(in2[0]) + in2[0][3];
	out[1] = in1.Dot(in2[1]) + in2[1][3];
	out[2] = in1.Dot(in2[2]) + in2[2][3];
}

void math::angle2vectors(const QAngle& angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

void math::AngleVectors(const float& angles, Vector& forward)
{
	float	sp, sy, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

QAngle math::CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	math::vector2angles(delta, angles);

	delta.Normalized();

	return angles;
}

QAngle math::CalcAngle2(const Vector& src, const Vector& dst)
{
	QAngle vAngle;
	Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
	double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

	vAngle.pitch = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
	vAngle.yaw = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
	vAngle.roll = 0.0f;

	if (delta.x >= 0.0)
	{
		vAngle.yaw += 180.0f;
	}

	return vAngle;
}

void math::angle2vectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up)
{
	float sr, sp, sy, cr, cp, cy;

	DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
	DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
	DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr * cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

void math::vector2angles(const Vector& forward, QAngle& angles)
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
		yaw = (atan2(forward[1], forward[0]) * 180 / DirectX::XM_PI);
		if (yaw < 0)
			yaw += 360;

		const auto tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / DirectX::XM_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

Vector math::CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

void math::vector2angles(const Vector& forward, Vector& up, QAngle& angles)
{
	Vector left = CrossProduct(up, forward);
	left.NormalizeInPlace();

	float forwardDist = forward.Length2D();

	if (forwardDist > 0.001f)
	{
		angles.pitch = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.yaw = atan2f(forward.y, forward.x) * 180 / M_PI;

		float upZ = (left.y * forward.x) - (left.x * forward.y);
		angles.roll = atan2f(left.z, upZ) * 180 / M_PI;
	}
	else
	{
		angles.pitch = atan2f(-forward.z, forwardDist) * 180 / M_PI;
		angles.yaw = atan2f(-left.x, left.y) * 180 / M_PI;
		angles.roll = 0;
	}
}

bool math::screen_transform(const Vector& in, Vector& out)
{
	auto exception_filter = [](int code, PEXCEPTION_POINTERS ex)
	{
		return EXCEPTION_EXECUTE_HANDLER;
	};

	__try
	{
		auto result = *(PDWORD)(globals::view_matrix::offset) + 988;
		if (!result)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		const auto& world_matrix = *(_D3DMATRIX*)result;

		const auto w = world_matrix.m[3][0] * in.x + world_matrix.m[3][1] * in.y + world_matrix.m[3][2] * in.z + world_matrix.m[3][3];
		if (w < 0.001f)
		{
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x = world_matrix.m[0][0] * in.x + world_matrix.m[0][1] * in.y + world_matrix.m[0][2] * in.z + world_matrix.m[0][3];
		out.y = world_matrix.m[1][0] * in.x + world_matrix.m[1][1] * in.y + world_matrix.m[1][2] * in.z + world_matrix.m[1][3];
		out.z = 0.0f;

		out.x /= w;
		out.y /= w;

		return true;
	}
	__except (exception_filter(GetExceptionCode(), GetExceptionInformation()))
	{
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}
}

bool math::world2screen(const Vector& in, Vector& out)
{
	if (!globals::view_matrix::has_offset)
		return false;

	if (!screen_transform(in, out))
		return false;

	int w, h;
	g::engine_client->GetScreenSize(w, h);

	out.x = (w / 2.0f) + (out.x * w) / 2.0f;
	out.y = (h / 2.0f) - (out.y * h) / 2.0f;

	return true;
}

static bool math::screen_transform2(const Vector& in, Vector& out) {
	static auto& w2sMatrix = g::engine_client->WorldToScreenMatrix();

	out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
	out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
	out.z = 0.0f;

	float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

	if (w < 0.001f) {
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	out.x /= w;
	out.y /= w;

	return true;
}
//--------------------------------------------------------------------------------
bool math::WorldToScreen2(const Vector& in, Vector& out) {
	if (screen_transform2(in, out)) {
		int w, h;
		g::engine_client->GetScreenSize(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
	return false;
}

float math::DotProduct(const float* v1, const float* v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void math::VectorRotate(const float* in1, const matrix3x4_t& in2, float* out)
{
	out[0] = DotProduct(in1, in2[0]);
	out[1] = DotProduct(in1, in2[1]);
	out[2] = DotProduct(in1, in2[2]);
}

void math::VectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	VectorRotate(&in1.x, in2, &out.x);
}

void math::VectorRotate(const Vector& in1, const QAngle& in2, Vector& out)
{
	matrix3x4_t matRotate;
	AngleMatrix(in2, matRotate);
	VectorRotate(in1, matRotate, out);
}

void math::MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			target[i][j] = source[i][j];
		}
	}
}

void math::MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2)
{
	matrix3x4_t out;
	if (&in1 == &out)
	{
		matrix3x4_t in1b;
		MatrixCopy(in1, in1b);
		MatrixMultiply(in1b, in2);
		return;
	}
	if (&in2 == &out)
	{
		matrix3x4_t in2b;
		MatrixCopy(in2, in2b);
		MatrixMultiply(in1, in2b);
		return;
	}
	out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
		in1[0][2] * in2[2][0];
	out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
		in1[0][2] * in2[2][1];
	out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
		in1[0][2] * in2[2][2];
	out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
		in1[0][2] * in2[2][3] + in1[0][3];
	out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
		in1[1][2] * in2[2][0];
	out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
		in1[1][2] * in2[2][1];
	out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
		in1[1][2] * in2[2][2];
	out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
		in1[1][2] * in2[2][3] + in1[1][3];
	out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
		in1[2][2] * in2[2][0];
	out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
		in1[2][2] * in2[2][1];
	out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
		in1[2][2] * in2[2][2];
	out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
		in1[2][2] * in2[2][3] + in1[2][3];

	in1 = out;
}

void inline math::SinCos(float radians, float* sine, float* cosine)
{
	_asm
	{
		fld DWORD PTR[radians]
		fsincos

		mov edx, DWORD PTR[cosine]
		mov eax, DWORD PTR[sine]

		fstp DWORD PTR[edx]
		fstp DWORD PTR[eax]
	}
}

void math::MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out)
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}

void math::AngleMatrix(const QAngle& angles, matrix3x4_t& matrix)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	// matrix = (YAW * PITCH) * ROLL
	matrix[0][0] = cp * cy;
	matrix[1][0] = cp * sy;
	matrix[2][0] = -sp;

	float crcy = cr * cy;
	float crsy = cr * sy;
	float srcy = sr * cy;
	float srsy = sr * sy;
	matrix[0][1] = sp * srcy - crsy;
	matrix[1][1] = sp * srsy + crcy;
	matrix[2][1] = sr * cp;

	matrix[0][2] = (sp * crcy + srsy);
	matrix[1][2] = (sp * crsy - srcy);
	matrix[2][2] = cr * cp;

	matrix[0][3] = 0.0f;
	matrix[1][3] = 0.0f;
	matrix[2][3] = 0.0f;
}

void math::AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix)
{
	AngleMatrix(angles, matrix);
	MatrixSetColumn(position, 3, matrix);
}

float math::GetRealDistanceFOV(const float& distance, const QAngle& current, const QAngle& aim)
{
	QAngle delta = aim - current;
	delta.NormalizeClamp();

	float hyp = sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f));

	Vector v_aim, v_current;
	angle2vectors(current, v_current);
	angle2vectors(aim, v_aim);

	float delta_distance = v_current.DistTo(v_aim);
	
	float real_distance = sin(D3DXToRadian(delta_distance)) * distance;

	float value = sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f)) * real_distance;

	value = std::clamp(value, -180.0f, 180.0f);

	return value;
}

float math::GetFovToPlayer(const QAngle& current_angles, const QAngle& aim_angles)
{
	QAngle delta = aim_angles - current_angles;
	delta.NormalizeClamp();

	return sqrtf(powf(delta.pitch, 2.0f) + powf(delta.yaw, 2.0f));
}

void math::angle2vectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	sp = static_cast<float>(sin(double(angles.pitch) * PIRAD));
	cp = static_cast<float>(cos(double(angles.pitch) * PIRAD));
	sy = static_cast<float>(sin(double(angles.yaw) * PIRAD));
	cy = static_cast<float>(cos(double(angles.yaw) * PIRAD));
	sr = static_cast<float>(sin(double(angles.roll) * PIRAD));
	cr = static_cast<float>(cos(double(angles.roll) * PIRAD));

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

void math::smooth(const float& amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& angles, const bool& humanize)
{
	angles = aim_angles;
	angles.NormalizeClamp();

	auto corrected_amount = amount;
	auto tickrate = 1.0f / g::global_vars->interval_per_tick;
	if (tickrate != 64.f)
	{
		//shooth 4
		//64 - 4
		//128 - x
		//x = 128*4/64
		//x = 8

		corrected_amount = tickrate * amount / 64.f;
	}

	if (corrected_amount < 1.1f)
		return;

	Vector aim_vector;
	angle2vectors(aim_angles, aim_vector);

	Vector current_vector;
	angle2vectors(current_angles, current_vector);

	auto delta = aim_vector - current_vector;
	if (humanize)
	{
		//delta.x += utils::random(-0.02f, 0.02f);
		delta.y += utils::random(-0.01f, 0.01f);
		delta.z += utils::random(-0.01f, 0.01f);
	}

	const auto smoothed = current_vector + delta / corrected_amount;

	vector2angles(smoothed, angles);
	angles.NormalizeClamp();
}

float math::AngleNormalize(float angle)
{
	angle = fmodf(angle, 360.0f);
	while (angle > 180)
		angle -= 360;

	while (angle < -180)
		angle += 360;

	return angle;
}

QAngle math::AngleNormalize(QAngle angle)
{
	angle.yaw = fmodf(angle.yaw, 360.0f);
	while (angle.yaw > 180)
		angle.yaw -= 360;

	while (angle.yaw < -180)
		angle.yaw += 360;

	return angle;
}