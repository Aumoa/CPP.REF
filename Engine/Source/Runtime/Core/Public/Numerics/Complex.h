// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Vector.h"
#include <initializer_list>

struct Complex : public Vector2
{
	/// <summary>
	/// Initialize new <see cref="Complex"/> instance.
	/// </summary>
	template<class... TArgs> requires std::constructible_from<Vector2, TArgs...>
	inline constexpr Complex(const TArgs&... args) : Vector2(args...)
	{
	}

	/// <summary>
	/// Rotate vector.
	/// </summary>
	constexpr Vector2 RotateVector(const Vector2& point) const
	{
		return
		{
			point[0] * X - point[1] * Y,
			point[0] * Y + point[1] * X
		};
	}
	
	template<class T>
	constexpr T TransformPoint(const T& point) const
	{
		return RotateVector(point);
	}

	template<class T>
	constexpr T TransformVector(const T& point) const
	{
		return RotateVector(point);
	}

	/// <summary>
	/// Get inversed quaternion.
	/// </summary>
	constexpr Complex GetInverse() const
	{
		return Complex(X, -Y);
	}

	constexpr Complex Concatenate(const Complex& rhs) const
	{
		return Complex(TransformPoint(rhs));
	}

	/// <summary>
	/// Get identity quaternion.
	/// </summary>
	static inline constexpr Complex GetIdentity()
	{
		return Complex(1.0f, 0.0f);
	}

	constexpr Matrix2x2 ToMatrix() const
	{
		return Matrix2x2
		{
			X, Y,
			-Y, X
		};
	}
};