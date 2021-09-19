// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include <initializer_list>
#include "Vector.h"
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
	constexpr Matrix4x4(std::initializer_list<Vector<float, Column()>> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix4x4"/> instance.
	/// </summary>
	/// <param name="rhs"> The initial copy value. </param>
	constexpr Matrix4x4(const Matrix<4, 4>& rhs) : Matrix(rhs)
	{
	}

	/// <summary>
	/// Get determinant value with SIMD functions.
	/// </summary>
	CORE_API float GetDeterminant() const;

	/// <summary>
	/// Get inversed matrix with SIMD functions.
	/// </summary>
	CORE_API Matrix4x4 GetInverse() const;

	/// <summary>
	/// Decompose transform components with SIMD functions.
	/// </summary>
	/// <param name="outTranslation"> The translation transform. </param>
	/// <param name="outScale"> The scale transform. </param>
	/// <param name="outRotation"> The rotation transform. </param>
	CORE_API void Decompose(Vector3& outTranslation, Vector3& outScale, Quaternion& outRotation) const;

	/// <summary>
	/// Transforms a 3D vector by the matrix.
	/// </summary>
	/// <param name="vector"> 3D vector. </param>
	/// <returns> Returns the transformed vector. </returns>
	CORE_API Vector3 TransformVector(const Vector3& vector) const;

	/// <summary>
	/// Transforms the 3D vector normal by the matrix.
	/// </summary>
	/// <param name="vector"> 3D normal vector. </param>
	/// <returns> Returns the transformed normal vector. </returns>
	CORE_API Vector3 TransformNormal(const Vector3& vector) const;

	/// <summary>
	/// Multiply two matrices with SIMD functions.
	/// </summary>
	/// <param name="lhs"> The first matrix. </param>
	/// <param name="rhs"> The second matrix. </param>
	/// <returns> Result matrix. </returns>
	CORE_API static Matrix4x4 Multiply(const Matrix4x4& lhs, const Matrix4x4& rhs);

	/// <summary>
	/// Make transform matrix that represents affine transformation.
	/// </summary>
	/// <param name="t"> The translation vector. </param>
	/// <param name="s"> The scale vector. </param>
	/// <param name="q"> The quaternion vector. </param>
	/// <returns> Result matrix. </returns>
	CORE_API static Matrix4x4 AffineTransformation(const Vector3& t, const Vector3& s, const Quaternion& q);

	/// <summary>
	/// Builds a view matrix for a Left-Handed coordinate system using a camera position, an up direction, and a camera direction.
	/// </summary>
	/// <param name="location"> Position of the camera. </param>
	/// <param name="dir"> Direction of the camera. </param>
	/// <param name="up"> Up direction of the camera. </param>
	/// <returns> Returns a view matrix that transforms a point from world space into view space. </returns>
	CORE_API static Matrix4x4 LookToLH(const Vector3& location, const Vector3& dir, const Vector3& up);

	/// <summary>
	/// Builds a LeftHanded perspective projection matrix based on a field of view.
	/// </summary>
	/// <param name="fovAngle"> Top-down field of view angle in radians. </param>
	/// <param name="aspectRatio"> Aspect ratio of view-space X:Y. </param>
	/// <param name="zNearPlane"> Distance to the near clipping plane. Must be greater than zero. </param>
	/// <param name="zFarPlane"> Distance to the far clipping plane. Must be greater than zero. </param>
	/// <returns> Returns the perspective projection matrix. </returns>
	CORE_API static Matrix4x4 PerspectiveFovLH(Radians fovAngle, float aspectRatio, float zNearPlane, float zFarPlane);

	/// <summary>
	/// Builds a matrix that rotates around the x-axis.
	/// </summary>
	/// <param name="angle"> Angle of rotation around the x-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin. </param>
	/// <returns> Returns the rotation matrix. </returns>
	CORE_API static Matrix4x4 RotationX(Radians angle);

	/// <summary>
	/// Builds a matrix that rotates around the y-axis.
	/// </summary>
	/// <param name="angle"> Angle of rotation around the y-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin. </param>
	/// <returns> Returns the rotation matrix. </returns>
	CORE_API static Matrix4x4 RotationY(Radians angle);

	/// <summary>
	/// Builds a matrix that rotates around the z-axis.
	/// </summary>
	/// <param name="angle"> Angle of rotation around the z-axis, in radians. Angles are measured clockwise when looking along the rotation axis toward the origin. </param>
	/// <returns> Returns the rotation matrix. </returns>
	CORE_API static Matrix4x4 RotationZ(Radians angle);

	/// <summary>
	/// Builds the identity matrix.
	/// </summary>
	/// <returns> Returns the identity matrix. </returns>
	inline static constexpr Matrix4x4 Identity()
	{
		return Matrix4x4
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	/// <summary>
	/// Builds a translation matrix from the specified offsets.
	/// </summary>
	/// <param name="translation"> Vector offset of translation. </param>
	/// <returns> Returns the translation matrix. </returns>
	inline static constexpr Matrix4x4 Translation(const Vector3& translation)
	{
		return Matrix4x4
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			translation.X, translation.Y, translation.Z, 1.0f
		};
	}
};