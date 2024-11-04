#pragma once
#include <algorithm>
#include <cmath>

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
	void normalize_clamp();
	bool is_zero();

	Vector to_vector() const;
    
    QAngle operator-() const
    {
        return QAngle(-this->pitch, -this->yaw, -this->roll);
    }

    QAngle operator-()
    {
        return QAngle(-this->pitch, -this->yaw, -this->roll);
    }

    QAngle operator+(float v) const
    {
        return QAngle(this->pitch + v, this->yaw + v, this->roll);
    }

    QAngle operator+(const QAngle& other) const
    {
        return QAngle(this->pitch + other.pitch, this->yaw + other.yaw, this->roll);
    }

    QAngle operator-(float v) const
    {
        return QAngle(this->pitch - v, this->yaw - v, this->roll);
    }

    QAngle operator*(float v) const
    {
        return QAngle(this->pitch * v, this->yaw * v, this->roll);
    }

    QAngle operator-(const QAngle& other) const
    {
        return QAngle(this->pitch - other.pitch, this->yaw - other.yaw, this->roll);
    }

    QAngle operator/(float other) const
    {
        return QAngle(this->pitch / other, this->yaw / other, this->roll);
    }

    QAngle& operator/=(float other)
    {
        this->pitch /= other;
        this->yaw /= other;
        
        return *this;
    }

    QAngle& operator*=(float other)
    {
        this->pitch *= other;
        this->yaw *= other;
        
        return *this;
    }

    QAngle& operator-=(const QAngle& other)
    {
        this->pitch -= other.pitch;
        this->yaw -= other.yaw;
        
        return *this;
    }

    QAngle& operator*=(const QAngle& other)
    {
        this->pitch *= other.pitch;
        this->yaw *= other.yaw;
        
        return *this;
    }

    QAngle& operator/=(const QAngle& other)
    {
        this->pitch /= other.pitch;
        this->yaw /= other.yaw;
        
        return *this;
    }

    QAngle& operator+=(const QAngle& other)
    {
        this->pitch += other.pitch;
        this->yaw += other.yaw;
        
        return *this;
    }

    static QAngle lerp(const QAngle& a, const QAngle& b, float t) {
        return QAngle(
            a.pitch + (b.pitch - a.pitch) * t,
            a.yaw + (b.yaw - a.yaw) * t,
            a.roll + (b.roll - a.roll) * t
        );
    }

    float length() const 
    {
        return std::sqrt(pitch * pitch + yaw * yaw + roll * roll);
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

