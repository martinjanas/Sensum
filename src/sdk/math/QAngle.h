#pragma once
#include <algorithm>

class Vector;

class QAngle
{
public:
	float pitch, yaw, roll;

	QAngle(float pitch, float yaw, float roll);
	QAngle();

	float operator[](size_t index) const;
	float& operator[](size_t index);

	void normalize();
	void clamp();
	void clamp_normalize();

	Vector to_vector() const;

	QAngle operator+(const float& v)
	{
		return QAngle(this->pitch + v, this->yaw + v, this->roll);
	}

	QAngle operator+(const QAngle& other)
	{
		return QAngle(this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll);
	}

	QAngle operator-(const float& v)
	{
		return QAngle(this->pitch - v, this->yaw - v, this->roll);
	}

	QAngle operator-(const QAngle& other)
	{
		return QAngle(this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll);
	}

	/*void Clamp()
	{
		pitch = std::clamp(pitch, -89.0f, 89.0f);
		yaw = std::clamp(yaw, -180.0f, 180.0f);
		roll = std::clamp(roll, 0.0f, 0.0f);
	}

	void Normalize()
	{
		for (auto& angle : { std::ref(pitch), std::ref(yaw), std::ref(roll) })
		{
			while (angle < -180.0f)
				angle += 360.0f;

			while (angle >= 180.0f)
				angle -= 360.0f;
		}
	}

	void ClampNormalize()
	{
		this->Normalize();
		this->Clamp();
	}*/
};
