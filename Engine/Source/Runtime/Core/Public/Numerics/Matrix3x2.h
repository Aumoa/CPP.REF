// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include "Mathematics/Radians.h"

struct Matrix3x2 : public Matrix<3, 2>
{
	/// <summary>
	/// Initialize new <see cref="Matrix3x2"/> instance.
	/// </summary>
	constexpr Matrix3x2() : Matrix()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix3x2"/> instance.
	/// </summary>
	/// <param name="values"> The initial values. </param>
	constexpr Matrix3x2(std::initializer_list<float> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix3x2"/> instance.
	/// </summary>
	/// <param name="values"> The initial vectors. </param>
	constexpr Matrix3x2(std::initializer_list<Vector<Column()>> values) : Matrix(values)
	{
	}

	inline static constexpr Matrix3x2 Identity()
	{
		return Matrix3x2
		{
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
		};
	}

	static constexpr Matrix3x2 Translation(const Vector2& translation)
	{
		return Matrix3x2
		{
			1.0f, 0.0f,
			0.0f, 1.0f,
			translation.X, translation.Y
		};
	}

	static constexpr Matrix3x2 Scale(const Vector2& scale)
	{
		return Matrix3x2
		{
			scale.X, 0.0f,
			0.0f, scale.Y,
			0.0f, 0.0f
		};
	}

	static Matrix3x2 Rotation(Radians angle)
	{
		float sin, cos;
		MathEx::SinCos(angle.Value, sin, cos);

		return Matrix3x2
		{
			cos, -sin,
			sin, cos,
			0.0f, 0.0f
		};
	}

	static constexpr Matrix3x2 Shear(const Vector2& shear)
	{
		return Matrix3x2
		{
			1.0f, shear.Y,
			shear.X, 1.0f,
			0.0f, 1.0f
		};
	}
};