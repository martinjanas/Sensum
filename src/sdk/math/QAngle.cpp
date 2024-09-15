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
	QAngle angles = *this;

	for (auto i = 0; i < 3; i++)
	{
		while (angles[i] < -180.0f)
			angles[i] += 360.0f;

		while (angles[i] > 180.0f)
			angles[i] -= 360.0f;
	}

	*this = angles;
}

void QAngle::clamp()
{
	QAngle angle = *this;

	if (!std::isfinite(angle.pitch))
		angle.pitch = 0.f;

	if (!std::isfinite(angle.yaw))
		angle.yaw = 0.f;

	if (!std::isfinite(angle.roll))
		angle.roll = 0.f;

	angle.pitch = std::clamp(angle.pitch, -89.f, 89.f);
	angle.yaw = std::clamp(std::remainder(angle.yaw, 360.f), -180.f, 180.f);
	angle.roll = 0.f;

	*this = angle;
}

void QAngle::clamp_normalize()
{
	QAngle angles = *this;

	angles.normalize();
	angles.clamp();

	*this = angles;
}

bool QAngle::is_zero()
{
	return pitch == 0.f || yaw == 0.f;
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

