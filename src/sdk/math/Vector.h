#pragma once
#include <iostream>
#include <format>
#include "../../thirdparty/ImGui/imgui.h"
#include "../helpers/matrix.h"

class Vector
{
public:

	Vector() : x(0.f), y(0.f), z(0.f) { };
	Vector(float _x, float _y) : x(_x), y(_y), z(0.f) { };
	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };


	float length() const
	{
		return std::sqrtf(x * x + y * y + z * z);
	}

	void normalize()
	{
		float length = this->length();

		if (length != 0)
			*this /= length;
	}

	std::string ToString()
	{
		auto str = std::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", x, y, z);

		return str;
	}

	float dot_product(const Vector& v) const noexcept
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	Vector operator-()
	{
		return Vector(-x, -y, -z);
	}

	Vector Transform(const matrix3x4_t& matrix) const 
	{
		return Vector{x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3],
					  x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3],
					  x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3]};
	}
	float operator[](const size_t& index) const
	{
		switch (index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		}
	}

	float& operator[](const size_t& index)
	{
		switch (index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		}
	}

	ImVec2 AsVec2()
	{
		if (this->x && this->y)
			return ImVec2(this->x, this->y);

		return ImVec2(0.f, 0.f);
	}

	Vector operator+(const Vector& other)
	{
		Vector& temp = *this;

		temp = Vector(temp.x + other.x, temp.y + other.y, temp.z + other.z);

		return temp;
	}

	Vector operator+(const float& value)
	{
		Vector& temp = *this;

		temp = Vector(temp.x + value, temp.y + value, temp.z + value);

		return temp;
	}

	Vector& operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector& operator+=(const float& value)
	{
		x += value;
		y += value;
		z += value;

		return *this;
	}

	Vector operator-(const Vector& other)
	{
		Vector& temp = *this;

		temp = Vector(temp.x - other.x, temp.y - other.y, temp.z - other.z);

		return temp;
	}

	Vector operator-(const float& value)
	{
		Vector& temp = *this;

		temp = Vector(temp.x - value, temp.y - value, temp.z - value);

		return temp;
	}

	Vector& operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector& operator-=(const float& value)
	{
		x -= value;
		y -= value;
		z -= value;

		return *this;
	}

	Vector operator*(const Vector& other)
	{
		Vector& temp = *this;

		temp = Vector(temp.x * other.x, temp.y * other.y, temp.z * other.z);

		return temp;
	}

	Vector operator*(const float& value)
	{
		Vector& temp = *this;

		temp = Vector(temp.x * value, temp.y * value, temp.z * value);

		return temp;
	}

	Vector& operator*=(const Vector& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	Vector operator*=(const float& value)
	{
		x *= value;
		y *= value;
		z *= value;

		return *this;
	}

	Vector operator/(const Vector& other)
	{
		Vector& temp = *this;

		temp = Vector(temp.x / other.x, temp.y / other.y, temp.z / other.z);

		return temp;
	}

	Vector& operator/=(const Vector& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}

	Vector& operator/=(const float& value)
	{
		x /= value;
		y /= value;
		z /= value;

		return *this;
	}

	float x;
	float y;
	float z;
};

