// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <numbers>
#include "PrimitiveTypes.h"
#include "EaseFunction.h"

struct Radians;

/// <summary>
/// Provide standard math functions.
/// </summary>
class CORENUMERICS_API MathEx abstract final
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
	template<class T>
	static inline constexpr T Mod(const T& x, const T& divisor) requires requires
	{
		{ x % divisor };
	}
	{
		return x % divisor;
	}

	/// <summary>
	/// Get inverted squared root value equals to 1.0 / Sqrt(x).
	/// </summary>
	/// <param name="x"> The argument value. </param>
	/// <returns> The computed value. </returns>
	static float InvSqrt(float x);

	template<class TL, class TR, class... T>
	static inline constexpr auto Min(const TL& Lhs, const TR& Rhs, const T&... Values)
	{
		if constexpr (sizeof...(T) > 0)
		{
			return Min((Lhs < Rhs ? Lhs : Rhs), Values...);
		}
		else
		{
			return (Lhs < Rhs ? Lhs : Rhs);
		}
	}

	template<class TL, class TR, class... T>
	static inline constexpr auto Max(const TL& Lhs, const TR& Rhs, const T&... Values)
	{
		if constexpr (sizeof...(T) > 0)
		{
			return Max((Lhs > Rhs ? Lhs : Rhs), Values...);
		}
		else
		{
			return (Lhs > Rhs ? Lhs : Rhs);
		}
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InSine)
	{
		return sin(1.5707963 * t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutSine)
	{
		return 1 + sin(1.5707963 * (--t));
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutSine)
	{
		return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InQuad)
	{
		return t * t;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutQuad)
	{
		return t * (2 - t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutQuad)
	{
		return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InCubic)
	{
		return t * t * t;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutCubic)
	{
		return 1 + (--t) * t * t;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutCubic)
	{
		return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InQuart)
	{
		t *= t;
		return t * t;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutQuart)
	{
		t = (--t) * t;
		return 1 - t * t;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutQuart)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InQuint)
	{
		double t2 = t * t;
		return t * t2 * t2;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutQuint)
	{
		double t2 = (--t) * t;
		return 1 + t * t2 * t2;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutQuint)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InExpo)
	{
		return (pow(2, 8 * t) - 1) / 255;
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutExpo)
	{
		return 1 - pow(2, -8 * t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutExpo)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InCirc)
	{
		return 1 - sqrt(1 - t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutCirc)
	{
		return sqrt(t);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutCirc)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InBack)
	{
		return t * t * (2.70158 * t - 1.70158);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutBack)
	{
		return 1 + (--t) * t * (2.70158 * t + 1.70158);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutBack)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InElastic)
	{
		double t2 = t * t;
		return t2 * t2 * sin(t * std::numbers::pi * 4.5);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutElastic)
	{
		double t2 = (t - 1) * (t - 1);
		return 1 - t2 * t2 * cos(t * std::numbers::pi * 4.5);
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutElastic)
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

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InBounce)
	{
		return pow(2, 6 * (t - 1)) * abs(sin(t * std::numbers::pi * 3.5));
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::OutBounce)
	{
		return 1 - pow(2, -6 * t) * abs(cos(t * std::numbers::pi * 3.5));
	}

	template<EEaseFunction Function>
	static double EaseFunction(double t) requires (Function == EEaseFunction::InOutBounce)
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

	template<class T>
	static T EaseFunction(EEaseFunction Function, T Value)
	{
#define EASE_CASE(Function) case EEaseFunction::Function: return (T)EaseFunction<EEaseFunction::Function>(Value);
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