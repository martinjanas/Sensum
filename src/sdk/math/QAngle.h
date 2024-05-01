#pragma once
#include <algorithm>

class QAngle
{
public:
	float pitch, yaw, roll;

	QAngle(float pitch, float yaw, float roll)
	{
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
	}

	QAngle()
	{
		this->pitch = 0.f;
		this->yaw = 0.f;
		this->roll = 0.f;
	}

	float operator[](size_t index) const
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

	float& operator[](size_t index)
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

	void Normalize()
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

	void Clamp()
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

	void ClampNormalize()
	{
		QAngle angles = *this;

		angles.Normalize();
		angles.Clamp();

		*this = angles;
	}
};

