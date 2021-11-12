// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <numbers>
#include "PrimitiveTypes.h"
#include "EaseFunction.h"

struct Radians;

/// <summary>
/// Provide standard math functions.
/// </summary>
class CORE_API MathEx abstract final
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

	static inline constexpr bool IsWithinInclusive(float v, float minv, float maxv)
	{
		return v >= minv && v <= maxv;
	}

	static constexpr float Round(float v)
	{
		if (v >= 0.0f)
		{
			return (float)(int32)(v + 0.5f);
		}
		else
		{
			return (float)(int32)(v - 0.5f);
		}
	}

	/// <summary>
	/// Represents small number.
	/// </summary>
	static inline constexpr float SmallNumber = 0.0001f;

private:
	double easeInSine(double t)
	{
		return sin(1.5707963 * t);
	}

	double easeOutSine(double t)
	{
		return 1 + sin(1.5707963 * (--t));
	}

	double easeInOutSine(double t)
	{
		return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
	}

	double easeInQuad(double t)
	{
		return t * t;
	}

	double easeOutQuad(double t)
	{
		return t * (2 - t);
	}

	double easeInOutQuad(double t)
	{
		return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
	}

	double easeInCubic(double t)
	{
		return t * t * t;
	}

	double easeOutCubic(double t)
	{
		return 1 + (--t) * t * t;
	}

	double easeInOutCubic(double t)
	{
		return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
	}

	double easeInQuart(double t)
	{
		t *= t;
		return t * t;
	}

	double easeOutQuart(double t)
	{
		t = (--t) * t;
		return 1 - t * t;
	}

	double easeInOutQuart(double t)
	{
		if (t < 0.5)
		{
			t *= t;
			return 8 * t * t;
		}
		else
		{
			t = (--t) * t;
			return 1 - 8 * t * t;
		}
	}

	double easeInQuint(double t)
	{
		double t2 = t * t;
		return t * t2 * t2;
	}

	double easeOutQuint(double t)
	{
		double t2 = (--t) * t;
		return 1 + t * t2 * t2;
	}

	double easeInOutQuint(double t)
	{
		double t2;
		if (t < 0.5)
		{
			t2 = t * t;
			return 16 * t * t2 * t2;
		}
		else
		{
			t2 = (--t) * t;
			return 1 + 16 * t * t2 * t2;
		}
	}

	double easeInExpo(double t)
	{
		return (pow(2, 8 * t) - 1) / 255;
	}

	double easeOutExpo(double t)
	{
		return 1 - pow(2, -8 * t);
	}

	double easeInOutExpo(double t)
	{
		if (t < 0.5)
		{
			return (pow(2, 16 * t) - 1) / 510;
		}
		else
		{
			return 1 - 0.5 * pow(2, -16 * (t - 0.5));
		}
	}

	double easeInCirc(double t)
	{
		return 1 - sqrt(1 - t);
	}

	double easeOutCirc(double t)
	{
		return sqrt(t);
	}

	double easeInOutCirc(double t)
	{
		if (t < 0.5)
		{
			return (1 - sqrt(1 - 2 * t)) * 0.5;
		}
		else
		{
			return (1 + sqrt(2 * t - 1)) * 0.5;
		}
	}

	double easeInBack(double t)
	{
		return t * t * (2.70158 * t - 1.70158);
	}

	double easeOutBack(double t)
	{
		return 1 + (--t) * t * (2.70158 * t + 1.70158);
	}

	double easeInOutBack(double t)
	{
		if (t < 0.5)
		{
			return t * t * (7 * t - 2.5) * 2;
		}
		else
		{
			return 1 + (--t) * t * 2 * (7 * t + 2.5);
		}
	}

	double easeInElastic(double t)
	{
		double t2 = t * t;
		return t2 * t2 * sin(t * std::numbers::pi * 4.5);
	}

	double easeOutElastic(double t)
	{
		double t2 = (t - 1) * (t - 1);
		return 1 - t2 * t2 * cos(t * std::numbers::pi * 4.5);
	}

	double easeInOutElastic(double t)
	{
		double t2;
		if (t < 0.45)
		{
			t2 = t * t;
			return 8 * t2 * t2 * sin(t * std::numbers::pi * 9);
		}
		else if (t < 0.55)
		{
			return 0.5 + 0.75 * sin(t * std::numbers::pi * 4);
		}
		else
		{
			t2 = (t - 1) * (t - 1);
			return 1 - 8 * t2 * t2 * sin(t * std::numbers::pi * 9);
		}
	}

	double easeInBounce(double t)
	{
		return pow(2, 6 * (t - 1)) * abs(sin(t * std::numbers::pi * 3.5));
	}

	double easeOutBounce(double t)
	{
		return 1 - pow(2, -6 * t) * abs(cos(t * std::numbers::pi * 3.5));
	}

	double easeInOutBounce(double t)
	{
		if (t < 0.5)
		{
			return 8 * pow(2, 8 * (t - 1)) * abs(sin(t * std::numbers::pi * 7));
		}
		else
		{
			return 1 - 8 * pow(2, -8 * t) * abs(sin(t * std::numbers::pi * 7));
		}
	}

public:
	template<class T>
	static T EaseFunction(EEaseFunction Function, T Value)
	{
#define EASE_CASE(NameDo) case EEaseFunction::NameDo: return (T)ease ## NameDo((double)Value);
		switch (Function)
		{
			EASE_CASE(InSine)
			EASE_CASE(OutSine)
			EASE_CASE(InOutSine)
			EASE_CASE(InQuad)
			EASE_CASE(OutQuad)
			EASE_CASE(InOutQuad)
			EASE_CASE(InCubic)
			EASE_CASE(OutCubic)
			EASE_CASE(InOutCubic)
			EASE_CASE(InQuart)
			EASE_CASE(OutQuart)
			EASE_CASE(InOutQuart)
			EASE_CASE(InQuint)
			EASE_CASE(OutQuint)
			EASE_CASE(InOutQuint)
			EASE_CASE(InExpo)
			EASE_CASE(OutExpo)
			EASE_CASE(InOutExpo)
			EASE_CASE(InCirc)
			EASE_CASE(OutCirc)
			EASE_CASE(InOutCirc)
			EASE_CASE(InBack)
			EASE_CASE(OutBack)
			EASE_CASE(InOutBack)
			EASE_CASE(InElastic)
			EASE_CASE(OutElastic)
			EASE_CASE(InOutElastic)
			EASE_CASE(InBounce)
			EASE_CASE(OutBounce)
			EASE_CASE(InOutBounce)
		default:
			return Value;
		}
#undef EASE_CASE
	}
};