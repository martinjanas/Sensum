#pragma once
#include "../math/Vector.h"

class VMatrix
{
public:

	VMatrix();
	VMatrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	)
	{
		Init(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		);
	}

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	VMatrix(const Vector& forward, const Vector& left, const Vector& up);

	// Construct from a 3x4 matrix
	//VMatrix(const matrix3x4_t& matrix3x4);

	// Set the values in the matrix.
	void  Init(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
	)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	// Initialize from a 3x4
	//void  Init(const matrix3x4_t& matrix3x4);

	// array access
	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float* Base()
	{
		return &m[0][0];
	}

	inline const float* Base() const
	{
		return &m[0][0];
	}

	void  SetLeft(const Vector& vLeft);
	void  SetUp(const Vector& vUp);
	void  SetForward(const Vector& vForward);

	void  GetBasisVectors(Vector& vForward, Vector& vLeft, Vector& vUp) const;
	void  SetBasisVectors(const Vector& vForward, const Vector& vLeft, const Vector& vUp);

	// Get/Set the translation.
	Vector& GetTranslation(Vector& vTrans) const;
	void  SetTranslation(const Vector& vTrans);

	void  PreTranslate(const Vector& vTrans);
	void  PostTranslate(const Vector& vTrans);

	/*matrix3x4_t& As3x4();
	const matrix3x4_t& As3x4() const;
	void  CopyFrom3x4(const matrix3x4_t& m3x4);
	void  Set3x4(matrix3x4_t& matrix3x4) const;*/

	bool  operator==(const VMatrix& src) const;
	bool  operator!=(const VMatrix& src) const { return !(*this == src); }

	// Access the basis vectors.
	Vector  GetLeft() const;
	Vector  GetUp() const;
	Vector  GetForward() const;
	Vector  GetTranslation() const;

	// Matrix->vector operations.
public:
	// Multiply by a 3D vector (same as operator*).
	void  V3Mul(const Vector& vIn, Vector& vOut) const;

	// Multiply by a 4D vector.
	//void  V4Mul( const Vector4D &vIn, Vector4D &vOut ) const;

	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector  ApplyRotation(const Vector& vVec) const;

	// Multiply by a vector (divides by w, assumes input w is 1).
	Vector  operator*(const Vector& vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector  VMul3x3(const Vector& vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector  VMul3x3Transpose(const Vector& vVec) const;

	// Multiply by the upper 3 rows.
	Vector  VMul4x3(const Vector& vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector  VMul4x3Transpose(const Vector& vVec) const;

	// Matrix->plane operations.
	//public:
	// Transform the plane. The matrix can only contain translation and rotation.
	//void  TransformPlane( const VPlane &inPlane, VPlane &outPlane ) const;

	// Just calls TransformPlane and returns the result.
	//VPlane  operator*(const VPlane &thePlane) const;

	// Matrix->matrix operations.
public:

	VMatrix& operator=(const VMatrix& mOther);

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix& other);

	// Add/Subtract two matrices.
	VMatrix  operator+(const VMatrix& other) const;
	VMatrix  operator-(const VMatrix& other) const;

	// Negation.
	VMatrix  operator-() const;

	// Return inverse matrix. Be careful because the results are undefined
	// if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix  operator~() const;

	// Matrix operations.
public:
	// Set to identity.
	void  Identity();
	bool  IsIdentity() const;
public:
	// The matrix.
	float  m[4][4];
};
