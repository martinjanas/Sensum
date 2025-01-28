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

    QAngle lerp(QAngle& aim_angle, float t);
    QAngle linear_smooth(QAngle& aim_angle, float speed);
    QAngle const_smooth(QAngle& aim_angle, float speed);

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

    float length() const 
    {
        return std::sqrt(pitch * pitch + yaw * yaw + roll * roll);
    }
};
