#include "QAngle.h"
#include "Vector.h"
#include "../math/math.h"

QAngle::QAngle(float pitch, float yaw, float roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

QAngle::QAngle()
{
	this->pitch = 0.f;
	this->yaw = 0.f;
	this->roll = 0.f;
}

void QAngle::normalize()
{
	pitch = std::remainderf(pitch, 360.0f);
	yaw = std::remainderf(yaw, 360.0f);
	roll = std::remainderf(roll, 360.0f);
}

void QAngle::clamp()
{
	if (!std::isfinite(pitch))
		pitch = 0.f;

	if (!std::isfinite(yaw))
		yaw = 0.f;

	if (!std::isfinite(roll))
		roll = 0.f;

	pitch = std::clamp(pitch, -89.f, 89.f);
	yaw = std::clamp(yaw, -180.f, 180.f);
	roll = std::clamp(roll, -50.f, 50.f);
}

void QAngle::normalize_clamp()
{
	normalize();
	clamp();
}

bool QAngle::is_zero()
{
	return pitch == 0.f && yaw == 0.f && roll == 0.f;
}

Vector QAngle::to_vector() const
{
	float pitch_rad = this->pitch * math::deg2rad;
	float yaw_rad = this->yaw * math::deg2rad;

	float sin_pitch = std::sinf(pitch_rad);
	float cos_pitch = std::cosf(pitch_rad);
	float sin_yaw = std::sinf(yaw_rad);
	float cos_yaw = std::cosf(yaw_rad);

	return Vector(cos_pitch * cos_yaw, cos_pitch * sin_yaw, -sin_pitch);
}

float QAngle::operator[](size_t index) const
{
	switch (index)
	{
	case 0:
		return this->pitch;
		break;
	case 1:
		return this->yaw;
		break;
	case 2:
		return this->roll;
		break;
	}
}

float& QAngle::operator[](size_t index)
{
	switch (index)
	{
	case 0:
		return this->pitch;
		break;
	case 1:
		return this->yaw;
		break;
	case 2:
		return this->roll;
		break;
	}
}

