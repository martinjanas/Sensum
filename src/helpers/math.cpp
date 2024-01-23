#include "math.h"
#include "utils.h"
#include "console.h"
#include "../settings/globals.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../features/features.h"

#include <d3dx9.h>
#include <D3dx9math.h>
#include <algorithm>
#include <cmath>

#include <format>

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
	
	Vector v_aim, v_current;

	math::angle2vectors(aim, v_aim);
	math::angle2vectors(current, v_current);

	float fov = v_current.DistTo(v_aim) * distance;

	fov /= 7.2f; //Scaling fov to match the slider

	if (settings::misc::print_fov)
		g::hud_chat->ChatPrintf(0, 0, std::format("fov: {:.1f}, distance: {:.1f}", fov, distance).c_str());

	return fov;
}

float math::GetFovToPlayer(const QAngle& current_angles, const QAngle& aim_angles)
{
	QAngle delta = aim_angles - current_angles;
	delta.NormalizeClamp();

	return std::hypotf(delta.pitch, delta.yaw);;
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

	corrected_amount = tickrate * amount / 64.f;

	if (corrected_amount < 1.1f)
		return;

	Vector aim_vector;
	angle2vectors(aim_angles, aim_vector);

	Vector current_vector;
	angle2vectors(current_angles, current_vector);

	auto delta = aim_vector - current_vector;
	if (humanize)
	{
		delta.x += utils::random(-0.02f, 0.02f);
		delta.y += utils::random(-0.01f, 0.01f);		
	}

	const auto smoothed = current_vector + delta / corrected_amount;

	vector2angles(smoothed, angles);
	angles.NormalizeClamp();
}
