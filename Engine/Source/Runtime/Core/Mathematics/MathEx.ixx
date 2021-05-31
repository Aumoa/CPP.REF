// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:MathEx;

import std.core;
import :PrimitiveTypes;

using namespace std;

struct Radians;

/// <summary>
/// Provide standard math functions.
/// </summary>
export class MathEx abstract final
{
public:
	/// <summary>
	/// Get sine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Sin(const Radians& x)
	{
		return sin(GetValue(x));
	}

	/// <summary>
	/// Get cosine value.
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float Cos(const Radians& x)
	{
		return cos(GetValue(x));
	}

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
	static float Sqrt(float x)
	{
		return sqrt(x);
	}

	/// <summary>
	/// Get mod value.
	/// </summary>
	/// <typeparam name="T"> The type of floating point. </typeparam>
	/// <param name="x"> The argument value. </param>
	/// <param name="divisor"> The divisor. </param>
	/// <returns> The computed value. </returns>
	template<floating_point T>
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
	template<signed_integral T>
	static inline constexpr T Mod(const T& x, const T& divisor)
	{
		return x % divisor;
	}

	/// <summary>
	/// Get inverted squared root value equals to 1.0 / Sqrt(x).
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float InvSqrt(float x)
	{
		int32 i = 0x5F3759DF - ((int32&)x >> 1);
		float ret = (float&)i;
		ret *= 1.5f - (x * 0.5f * ret * ret);
		return ret;
	}

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

private:
	// Implemented in Radians.ixx
	static float GetValue(const Radians& v);
};