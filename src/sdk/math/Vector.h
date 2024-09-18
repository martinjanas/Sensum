#pragma once
#include <iostream>
#include <format>
#include "../../thirdparty/ImGui/imgui.h"
#include "../helpers/matrix.h"
#include "../math/math.h"
#include "../math/QAngle.h"

class Vector
{
public:
	float x, y, z;

	Vector() { invalidate(); }
	Vector(float _x, float _y) : x(_x), y(_y), z(0.f) { };
	Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };
	Vector(const float* clr) : x(clr[0]), y(clr[1]), z(clr[2]) { };

	void invalidate();

	//Helper functions
	float length() const;
	float length_sqr() const;
	float dot_product(const Vector& v) const;
	float dist_to(const Vector& other) const;
	void normalize();
	Vector transform(const matrix3x4_t& matrix) const;
	bool is_valid() const;
	const char* to_string() const;
	ImVec2 as_vec2() const;
	QAngle to_qangle();

	//Operator overloads
	float operator[](const size_t& index) const;
	float& operator[](const size_t& index);

	Vector operator+(const Vector& other) const {
		return Vector(x + other.x, y + other.y, z + other.z);
	}

	Vector operator+(float scalar) const {
		return Vector(x + scalar, y + scalar, z + scalar);
	}

	Vector operator-(float scalar) const {
		return Vector(x - scalar, y - scalar, z - scalar);
	}

	Vector operator-(const Vector& other) const {
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector operator*(float scalar) const {
		return Vector(x * scalar, y * scalar, z * scalar);
	}

	Vector operator/(float scalar) const {
		if (scalar != 0) {
			return Vector(x / scalar, y / scalar, z / scalar);
		}
		
		return *this;
	}

	Vector& operator+=(const Vector& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector& operator-=(const Vector& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	Vector& operator+=(float scalar) {
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	Vector& operator-=(float scalar) {
		x -= scalar;
		y -= scalar;
		z -= scalar;
		return *this;
	}

	Vector& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector& operator/=(float scalar) {
		if (scalar != 0) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
		}
		// Handle division by zero as appropriate
		return *this;
	}

	//TODO: Implement more const operators
};

