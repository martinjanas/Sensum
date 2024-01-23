#pragma once

#include <cmath>
#include <limits>
#include <sstream>

class Vector
{
public:
	Vector(void)
	{
		Invalidate();
	}
	Vector(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	Vector(float X, float Y)
	{
		x = X;
		y = Y;
		z = 0;
	}
	Vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}
	bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	void Zero()
	{
		x = y = z = 0.0f;
	}

	bool operator==(const Vector& src) const
	{
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
	bool operator!=(const Vector& src) const
	{
		return (src.x != x) || (src.y != y) || (src.z != z);
	}

	Vector& operator+=(const Vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector& operator+=(const float& v)
	{
		x += v; y += v; z += v;

		return *this;
	}
	Vector& operator-=(const Vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	Vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}
	Vector& operator*=(const Vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vector& operator/=(const Vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	Vector& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}
	Vector& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	Vector& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	void NormalizeInPlace()
	{
		*this = Normalized();
	}
	Vector Normalized() const
	{
		Vector res = *this;
		float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	float DistTo(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}
	float DistToSqr(const Vector& vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}
	float Dot(const Vector& vOther) const
	{
		return (x * vOther.x + y * vOther.y + z * vOther.z);
	}
	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}
	float Length2D() const
	{
		return sqrt(x * x + y * y);
	}

	Vector& operator=(const Vector& vOther)
	{
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}

	Vector operator-(void) const
	{
		return Vector(-x, -y, -z);
	}
	Vector operator+(const Vector& v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator+(const float& v) const
	{
		return Vector(x + v, y + v, z + v);
	}
	Vector operator-(const Vector& v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator-(const float& v) const
	{
		return Vector(x - v, y - v, z - v);
	}
	Vector operator*(float fl) const
	{
		return Vector(x * fl, y * fl, z * fl);
	}
	Vector operator*(const Vector& v) const
	{
		return Vector(x * v.x, y * v.y, z * v.z);
	}
	Vector operator/(float fl) const
	{
		return Vector(x / fl, y / fl, z / fl);
	}
	Vector operator/(const Vector& v) const
	{
		return Vector(x / v.x, y / v.y, z / v.z);
	}

	void CrossProduct(const Vector& a, const Vector& b, Vector& result)
	{
		result.x = a.y * b.z - a.z * b.y;
		result.y = a.z * b.x - a.x * b.z;
		result.z = a.x * b.y - a.y * b.x;
	}

	Vector Cross(const Vector& vOther)
	{
		Vector res;
		CrossProduct(*this, vOther, res);
		return res;
	}

	float x, y, z;
};

inline Vector operator*(float lhs, const Vector& rhs)
{
	return rhs * lhs;
}
inline Vector operator/(float lhs, const Vector& rhs)
{
	return rhs / lhs;
}

class __declspec(align(16)) VectorAligned : public Vector
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

public:
	explicit VectorAligned(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	VectorAligned& operator=(const Vector& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	VectorAligned& operator=(const VectorAligned& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;
};

// 2D Vector
class Vector2D
{
public:
	// Members
	float x, y;

	// Construction/destruction:
	Vector2D(void);
	Vector2D(float X, float Y);
	Vector2D(float* clr);

	Vector2D(const Vector2D& vOther)
	{
		x = vOther.x; y = vOther.y;
	}

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector2D& v) const;
	bool operator!=(const Vector2D& v) const;

	// arithmetic operations
	Vector2D& operator+=(const Vector2D& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	Vector2D& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	Vector2D& operator*=(const Vector2D& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2D& operator/=(const Vector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	Vector2D& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	float Length() const;

	// Get the vector's magnitude squared.
	float LengthSqr(void) const
	{
		return (x * x + y * y);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance);
	}

	float NormalizeInPlace();
	Vector2D Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector2D const& boxmin, Vector2D const& boxmax);

	// Get the distance from this vector to the other one.
	float DistTo(const Vector2D& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .
	// may be able to tidy this up after switching to VC7
	float DistToSqr(const Vector2D& vOther) const
	{
		Vector2D delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const Vector2D& a, const Vector2D& b, float scalar);

	// Dot product.
	float Dot(const Vector2D& vOther) const;

	// assignment
	Vector2D& operator=(const Vector2D& vOther);

	// 2d
	float Length2D(void) const;
	float Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	Vector2D  ProjectOnto(const Vector2D& onto);

	// copy constructors
	// Vector2D(const Vector2D &vOther);

	// arithmetic operations
	Vector2D operator-(void) const;

	Vector2D operator+(const Vector2D& v) const;
	Vector2D operator-(const Vector2D& v) const;
	Vector2D operator*(const Vector2D& v) const;
	Vector2D operator/(const Vector2D& v) const;
	Vector2D operator*(float fl) const;
	Vector2D operator/(float fl) const;

	// Cross product between two vectors.
	Vector2D Cross(const Vector2D& vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector2D Min(const Vector2D& vOther) const;
	Vector2D Max(const Vector2D& vOther) const;
};

class Vector4D
{
public:
	// Members
	float x, y, z, w;

	// Construction/destruction:
	Vector4D(void);
	Vector4D(float X, float Y, float Z, float W);
	Vector4D(float* clr);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);
	void Zero(); ///< zero out a vector

				 // equality
	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	// arithmetic operations
	Vector4D& operator+=(const Vector4D& v)
	{
		x += v.x; y += v.y; z += v.z; w += v.w;
		return *this;
	}

	Vector4D& operator-=(const Vector4D& v)
	{
		x -= v.x; y -= v.y; z -= v.z; w -= v.w;
		return *this;
	}

	Vector4D& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		w *= fl;
		return *this;
	}

	Vector4D& operator*=(const Vector4D& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	Vector4D& operator/=(const Vector4D& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	// this ought to be an opcode.
	Vector4D& operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		w += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector4D& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		w /= fl;
		return *this;
	}
	Vector4D& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		w -= fl;
		return *this;
	}

	// negate the vector components
	void Negate();

	// Get the vector's magnitude.
	float Length() const;

	// Get the vector's magnitude squared.
	float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance &&
			w > -tolerance && w < tolerance);
	}

	float NormalizeInPlace();
	Vector4D Normalized() const;
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(Vector4D const& boxmin, Vector4D const& boxmax);

	// Get the distance from this vector to the other one.
	float DistTo(const Vector4D& vOther) const;

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .
	// may be able to tidy this up after switching to VC7
	float DistToSqr(const Vector4D& vOther) const
	{
		Vector4D delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;
		delta.w = w - vOther.w;

		return delta.LengthSqr();
	}

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void MulAdd(const Vector4D& a, const Vector4D& b, float scalar);

	// Dot product.
	float Dot(const Vector4D& vOther) const;

	// assignment
	Vector4D& operator=(const Vector4D& vOther);

	// 2d
	float Length2D(void) const;
	float Length2DSqr(void) const;

	/// Get the component of this vector parallel to some other given vector
	Vector4D  ProjectOnto(const Vector4D& onto);

	// copy constructors
	// Vector4D(const Vector4D &vOther);

	// arithmetic operations
	Vector4D operator-(void) const;

	Vector4D operator+(const Vector4D& v) const;
	Vector4D operator-(const Vector4D& v) const;
	Vector4D operator*(const Vector4D& v) const;
	Vector4D operator/(const Vector4D& v) const;
	Vector4D operator*(float fl) const;
	Vector4D operator/(float fl) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector4D Min(const Vector4D& vOther) const;
	Vector4D Max(const Vector4D& vOther) const;
};