// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "PrimitiveTypes.h"

struct Radians;

/// <summary>
/// Provide standard math functions.
/// </summary>
class MathEx abstract final
{
public:
	/// <summary>
	/// Get sine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Sin(const Radians& x);

	/// <summary>
	/// Get cosine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Cos(const Radians& x);

	/// <summary>
	/// Get tangent value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Tan(const Radians& x);

	/// <summary>
	/// Get arc sine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static Radians Asin(float x);

	/// <summary>
	/// Get arc cosine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static Radians Acos(float x);

	/// <summary>
	/// Get arc tangent value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Atan(float x);

	/// <summary>
	/// Get principal value of arc tangent of y/x.
	/// </summary>
	/// <param name="y"> The argument value. </param>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Atan2(float y, float x);

	/// <summary>
	/// Get sine and cosine values.
	/// </summary>
	/// <param name="y"> The argument value. </param>
	/// <param name="osin"> The return parameter of sine value. </param>
	/// <param name="ocos"> The return parameter of cosine value.  </param>
	static void SinCos(const Radians& x, float& osin, float& ocos);

	/// <summary>
	/// Get absolute value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	template<class T>
	static inline constexpr T Abs(const T& lhs)
	{
		return lhs < (T)0 ? -lhs : lhs;
	}

	/// <summary>
	/// Get squared root value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Sqrt(float x);

	/// <summary>
	/// Get mod value.
	/// </summary>
	/// <typeparam name="T"> The type of floating point. </typeparam>
	/// <param name="x"> The argument value. </param>
	/// <param name="divisor"> The divisor. </param>
	/// <returns> The computed value. </returns>
	template<std::floating_point T>
	static inline constexpr T Mod(const T& x, const T& divisor)
	{
		T exp = x / divisor;
		int64 exp_n = (int64)exp;
		T re = exp - (T)exp_n;
		return divisor * re;
	}

	/// <summary>
	/// Get mod value.
	/// </summary>
	/// <typeparam name="T"> The type of integer. </typeparam>
	/// <param name="x"> The argument value. </param>
	/// <param name="divisor"> The divisor. </param>
	/// <returns> The computed value. </returns>
	template<std::signed_integral T>
	static inline constexpr T Mod(const T& x, const T& divisor)
	{
		return x % divisor;
	}

	/// <summary>
	/// Get inverted squared root value equals to 1.0 / Sqrt(x).
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float InvSqrt(float x);

	/// <summary>
	/// Get minimum value of arguments.
	/// </summary>
	/// <param name="lhs"> The first value. </param>
	/// <param name="rhs"> The second value. </param>
	/// <returns> The minimum value. </returns>
	template<class T>
	static inline constexpr T Min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	/// <summary>
	/// Get maximum value of arguments.
	/// </summary>
	/// <param name="lhs"> The first value. </param>
	/// <param name="rhs"> The second value. </param>
	/// <returns> The maximum value. </returns>
	template<class T>
	static inline constexpr T Max(const T& lhs, const T& rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	/// <summary>
	/// Clamp value between lhs and rhs.
	/// </summary>
	template<class T>
	static inline constexpr T Clamp(const T& value, const T& lhs, const T& rhs)
	{
		return Max(Min(value, rhs), lhs);
	}

	/// <summary>
	/// Represents small number.
	/// </summary>
	static inline constexpr float SmallNumber = 0.0001f;
};