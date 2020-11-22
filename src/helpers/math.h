#pragma once

#include "../valve_sdk/sdk.hpp"

#include <DirectXMath.h>

#define CHECK_VALID( _v) 0

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

#define PIRAD 0.01745329251f
#define PI 3.14159265358979323846f

#define _USE_MATH_DEFINES
#include <math.h>

namespace math
{
	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void angle2vectors(const QAngle& angles, Vector& forward);
	void AngleVectors(const float& angles, Vector& forward);
	QAngle CalcAngle(Vector src, Vector dst);
	QAngle CalcAngle2(const Vector& src, const Vector& dst);
	void angle2vectors(const QAngle& angles, Vector& forward, Vector& right, Vector& up);
	Vector CrossProduct(const Vector& a, const Vector& b);
	void vector2angles(const Vector& forward, QAngle& angles);
	void vector2angles(const Vector& forward, Vector& up, QAngle& angles);
	bool screen_transform(const Vector& in, Vector& out);
	bool world2screen(const Vector& in, Vector& out);
	bool screen_transform2(const Vector& in, Vector& out);
	bool WorldToScreen2(const Vector& in, Vector& out);
	void correct_movement(CUserCmd* cmd, const QAngle& old_angles);
	void correct_movement(CUserCmd* cmd, const QAngle& old_angles, const float& old_forward, const float& old_sidemove);
	void MovementFix(CUserCmd* m_Cmd, QAngle wish_angle, QAngle old_angles);
	void FixAngles(QAngle& angles);
	float DotProduct(const float* v1, const float* v2);
	void VectorRotate(const float* in1, const matrix3x4_t& in2, float* out);
	void VectorRotate(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	void VectorRotate(const Vector& in1, const QAngle& in2, Vector& out);
	void MatrixCopy(const matrix3x4_t& source, matrix3x4_t& target);
	void MatrixMultiply(matrix3x4_t& in1, const matrix3x4_t& in2);
	void AngleMatrix(const QAngle& angles, const Vector& position, matrix3x4_t& matrix);
	void AngleMatrix(const QAngle& angles, matrix3x4_t& matrix);
	void MatrixSetColumn(const Vector& in, int column, matrix3x4_t& out);
	void inline SinCos(float radians, float* sine, float* cosine);
	float GetRealDistanceFOV(const float& distance, const QAngle& current, const QAngle& aim);
	float GetFovToPlayer(const QAngle& viewAngle, const QAngle& aimAngle);
	void angle2vectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);
	void smooth(const float& amount, const QAngle& current_angles, const QAngle& aim_angles, QAngle& angle, const bool& humanize = false);
	float AngleNormalize(float angle);
	QAngle AngleNormalize(QAngle angle);
	float Magnitude(Vector a);
	Vector Normalize(Vector value);
	Vector ClampMagnitude(Vector vector, float maxLength);
}