// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include <initializer_list>
#include "Vector.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Mathematics/Radians.h"

struct Matrix4x4 : public Matrix<4, 4>
{
	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	constexpr Matrix4x4() : Matrix()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	/// <param name="values"> The initial values. </param>
	constexpr Matrix4x4(std::initializer_list<float> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	/// <param name="values"> The initial vectors. </param>
	constexpr Matrix4x4(std::initializer_list<Vector<Column()>> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Get determinant value with SIMD functions.
	/// </summary>
	float GetDeterminant() const;

	/// <summary>
	/// Get inversed matrix with SIMD functions.
	/// </summary>
	Matrix4x4 GetInverse() const;

	/// <summary>
	/// Decompose transform components with SIMD functions.
	/// </summary>
	/// <param name="outTranslation"> The translation transform. </param>
	/// <param name="outScale"> The scale transform. </param>
	/// <param name="outRotation"> The rotation transform. </param>
	void Decompose(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const;

	Vector3 TransformVector(const Vector3& vector) const;

	/// <summary>
	/// Multiply two matrices with SIMD functions.
	/// </summary>
	/// <param name="lhs"> The first matrix. </param>
	/// <param name="rhs"> The second matrix. </param>
	/// <returns> Result matrix. </returns>
	static Matrix4x4 Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs);

	/// <summary>
	/// Make transform matrix that represents affine transformation.
	/// </summary>
	/// <param name="t"> The translation vector. </param>
	/// <param name="s"> The scale vector. </param>
	/// <param name="q"> The quaternion vector. </param>
	/// <returns> Result matrix. </returns>
	static Matrix4x4 AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q);

	static Matrix4x4 LookToLH(const Vector3& location, const Vector3& dir, const Vector3& up);
	static Matrix4x4 PerspectiveFovLH(Radians fovAngle, float aspectRatio, float zNearPlane, float zFarPlane);
	inline static constexpr Matrix4x4 Identity();
};

inline constexpr Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}