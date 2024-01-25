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

	Vector transform(const mat3x4_t& mat) noexcept
	{
		return Vector(dot_product({ mat[0][0], mat[0][1], mat[0][2] }) + mat[0][3],
			dot_product({ mat[1][0], mat[1][1], mat[1][2] }) + mat[1][3],
			dot_product({ mat[2][0], mat[2][1], mat[2][2] }) + mat[2][3]);
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
		return ImVec2(this->x, this->y);
	}

	Vector operator+(const Vector& other)
	{
		Vector& temp = *this;

		temp = Vector(temp.x + other.x, temp.y + other.y, temp.z + other.z);

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

