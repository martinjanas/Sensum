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

QAngle QAngle::lerp(QAngle& aim_angle, float t)
{
	QAngle out_angle;

	t = std::clamp(t, 0.0f, 1.0f);

	out_angle.pitch = std::lerp(this->pitch, aim_angle.pitch, t);
	out_angle.yaw = std::lerp(this->yaw, aim_angle.yaw, t);
	out_angle.normalize_clamp();

	return out_angle;
}

QAngle QAngle::linear_smooth(QAngle& aim_angle, float speed)
{
	QAngle out_angle;

	if (speed <= 1.f)
		return aim_angle;

	QAngle delta = aim_angle - *this;
	delta.normalize_clamp();

	out_angle = *this + delta / speed;
	out_angle.normalize_clamp();

	return out_angle;
}

QAngle QAngle::const_smooth(QAngle& aim_angle, float speed)
{
	QAngle out_angle = *this;

	if (speed <= 1.f)
		return aim_angle;

	float max_smooth_step = 10.0f;
	float smooth_step = (max_smooth_step / (speed + 0.1f)) * 0.015625f;

	for (int i = 0; i < 3; ++i) 
	{
		float current = out_angle[i];
		float target = aim_angle[i];
		float delta = target - current;

		if (std::fabs(delta) < smooth_step)
			out_angle[i] = aim_angle[i]; 
		else out_angle[i] += (delta > 0 ? smooth_step : -smooth_step);
	}

	out_angle.normalize_clamp();

	return out_angle;
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

