// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "Vector2.h"
#include <initializer_list>

struct Complex : public Vector2
{
	/// <summary>
	/// Initialize new <see cref="Complex"/> instance.
	/// </summary>
	inline constexpr Complex()
	{
	}

	inline Complex(const Radians& rot)
	{
		MathEx::SinCos(rot, Values[0], Values[1]);
	}

	inline constexpr Complex(float x, float y) : Vector2(x, y)
	{
	}

	/// <summary>
	/// Initialize new <see cref="Complex"/> instance.
	/// </summary>
	/// <param name="initializer"> The initializer to initialize vector values. </param>
	inline constexpr Complex(std::initializer_list<float> initializer)
	{
		for (size_t i = 0; i < MathEx::Min(initializer.size(), Num()); ++i)
		{
			Values[i] = initializer.begin()[i];
		}
	}

	/// <summary>
	/// Initialize new <see cref="Complex"/> instance.
	/// </summary>
	/// <param name="copy"> The copy vector to initialize vector values. </param>
	inline constexpr Complex(const Vector<2>& copy)
	{
		for (size_t i = 0; i < Num(); ++i)
		{
			Values[i] = copy[i];
		}
	}

	/// <summary>
	/// Get X component.
	/// </summary>
	/// <returns> Reference of X component. </returns>
	inline constexpr const float& X() const
	{
		return Values[0];
	}

	/// <summary>
	/// Get Y component.
	/// </summary>
	/// <returns> Reference of Y component. </returns>
	inline constexpr const float& Y() const
	{
		return Values[1];
	}

	/// <summary>
	/// Get X component.
	/// </summary>
	/// <returns> Reference of X component. </returns>
	inline float& X()
	{
		return Values[0];
	}

	/// <summary>
	/// Get Y component.
	/// </summary>
	/// <returns> Reference of Y component. </returns>
	inline float& Y()
	{
		return Values[1];
	}

	/// <summary>
	/// Rotate vector.
	/// </summary>
	constexpr Vector2 RotateVector(const Vector2& point) const
	{
		return
		{
			point[0] * X() - point[1] * Y(),
			point[0] * Y() + point[1] * X()
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
		return Complex(X(), -Y());
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
			X(), Y(),
			-Y(), X()
		};
	}
};