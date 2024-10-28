#include "Vector.h"
#include <limits>

void Vector::invalidate()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

float Vector::length() const
{
	return std::sqrtf(x * x + y * y + z * z);
}

float Vector::length_sqr() const
{
	return (x * x + y * y + z * z);
}

float Vector::dot_product(const Vector& v) const
{
	return (x * v.x + y * v.y + z * v.z);
}

Vector Vector::cross(const Vector& other) const 
{
	return Vector(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

float Vector::dist_to(const Vector& other) const
{
	Vector delta;

	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

void Vector::normalize()
{
	float length = this->length();

	if (length != 0)
		*this /= length;
}

Vector Vector::transform(const matrix3x4_t& matrix) const
{
	return Vector(x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3],
		x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3],
		x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3]);
}

bool Vector::is_valid() const
{
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

const char* Vector::to_string() const
{
	return std::format("x: {:.2f}, y: {:.2f}, z: {:.2f}", x, y, z).c_str();
}

ImVec2 Vector::as_vec2() const
{
	return ImVec2(this->x, this->y);
}

QAngle Vector::to_qangle()
{
	QAngle angles;

	// Calculate yaw angle
	if (this->x == 0 && this->y == 0)
		angles.yaw = (this->z > 0) ? 270.0f : 90.0f;
	else
	{
		angles.yaw = atan2f(this->y, this->x) * math::rad2deg;
		if (angles.yaw < 0)
			angles.yaw += 360.0f;
	}

	// Calculate pitch angle
	float projection_length = sqrtf(this->x * this->x + this->y * this->y);
	angles.pitch = atan2f(-this->z, projection_length) * math::rad2deg;
	if (angles.pitch < 0)
		angles.pitch += 360.0f;

	// Set roll angle to 0
	angles.roll = 0.0f;

	return angles;
}

//Operator overloads
float Vector::operator[](const size_t& index) const
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

float& Vector::operator[](const size_t& index)
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

