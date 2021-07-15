// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Matrix.h"
#include "Vector2.h"
#include "Mathematics/Radians.h"

struct Matrix2x2 : public Matrix<2, 2>
{
	/// <summary>
	/// Initialize new <see cref="Matrix2x2"/> instance.
	/// </summary>
	constexpr Matrix2x2() : Matrix()
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix2x2"/> instance.
	/// </summary>
	/// <param name="values"> The initial values. </param>
	constexpr Matrix2x2(std::initializer_list<float> values) : Matrix(values)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Matrix2x2"/> instance.
	/// </summary>
	/// <param name="values"> The initial vectors. </param>
	constexpr Matrix2x2(std::initializer_list<Vector<Column()>> values) : Matrix(values)
	{
	}

	constexpr Matrix2x2(const Matrix<2, 2>& rhs) : Matrix(rhs)
	{
	}

	inline static constexpr Matrix2x2 Identity()
	{
		return Matrix2x2
		{
			1.0f, 0.0f,
			0.0f, 1.0f,
		};
	}

	static constexpr Matrix2x2 Scale(const Vector2& scale)
	{
		return Matrix2x2
		{
			scale.X(), 0.0f,
			0.0f, scale.Y()
		};
	}

	static Matrix2x2 Rotation(const Radians& angle)
	{
		float sin, cos;
		MathEx::SinCos(angle.Value, sin, cos);

		return Matrix2x2
		{
			cos, -sin,
			sin, cos
		};
	}

	static constexpr Matrix2x2 Shear(const Vector2& shear)
	{
		return Matrix2x2
		{
			1.0f, shear.Y(),
			shear.X(), 1.0f
		};
	}

	constexpr Vector2 TransformPoint(const Vector2& point) const
	{
		return
		{
			point[0] * V[0][0] + point[1] * V[1][0],
			point[0] * V[0][1] + point[1] * V[1][1]
		};
	}

	constexpr Vector2 TransformVector(const Vector2& point) const { return TransformPoint(point); }

	constexpr Matrix2x2 GetInverse() const
	{
		float a = V[0][0], b = V[0][1], c = V[1][0], d = V[1][1];
		float det = a * d - b * c;
		float invdet = 1.0f / det;
		return
		{
			d * invdet, -b * invdet,
			-c * invdet, a * invdet
		};
	}
};