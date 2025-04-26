// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "EaseFunction.h"
#include "Platform/PlatformMacros.h"
#include <utility>
#include <numbers>
#include <cmath>

namespace Ayla
{
	template<std::floating_point T>
	struct Radians;

	/// <summary>
	/// Provide standard math functions.
	/// </summary>
	class CORE_API Math
	{
		Math() = delete;

	public:
		/// <summary>
		/// Get sine value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static T Sin(const Radians<T>& x);

		/// <summary>
		/// Get cosine value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static T Cos(const Radians<T>& x);

		/// <summary>
		/// Get tangent value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static T Tan(const Radians<T>& x);

		/// <summary>
		/// Get arc sine value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static Radians<T> Asin(const T& x);

		/// <summary>
		/// Get arc cosine value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static Radians<T> Acos(const T& x);

		/// <summary>
		/// Get arc tangent value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static T Atan(const T& x);

		/// <summary>
		/// Get principal value of arc tangent of y/x.
		/// </summary>
		/// <param name="y"> The argument value. </param>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static T Atan2(const T& y, const T& x);

		/// <summary>
		/// Get sine and cosine values.
		/// </summary>
		/// <param name="y"> The argument value. </param>
		/// <param name="osin"> The return parameter of sine value. </param>
		/// <param name="ocos"> The return parameter of cosine value. </param>
		template<std::floating_point T>
		static constexpr void SinCos(const Radians<T>& x, T& osin, T& ocos);

		/// <summary>
		/// Get absolute value.
		/// </summary>
		/// <param name="x"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<class T>
		inline static constexpr T Abs(const T& lhs)
		{
			return lhs < (T)0 ? -lhs : lhs;
		}

		/// <summary>
		/// Get squared root value.
		/// </summary>
		/// <param name="value"> The argument value. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static constexpr T Sqrt(const T& value, const T& guess = (T)1)
		{
			if constexpr (std::is_constant_evaluated())
			{
				return (guess * guess - value) < 1e-10 && (value - guess * guess) < 1e-10
					? guess
					: Sqrt(value, (guess + value / guess) / 2.0);

			}
			else
			{
				return sqrt(value);
			}
		}

		/// <summary>
		//  Returns an estimate of the reciprocal square root of a specified number.
		/// </summary>
		/// <param name="x"> The number whose reciprocal square root is to be estimated. </param>
		/// <returns> An estimate of the reciprocal square root x. </returns>
		template<std::floating_point T>
		inline static constexpr T ReciprocalSqrtEstimate(const T& x)
		{
			if constexpr (std::same_as<T, float>)
			{
				union SingleToInt32
				{
					float v1;
					int32 v2;
				};

				int32 i = 0x5F3759DF - (SingleToInt32{ .v1 = x }.v2 >> 1);
				float ret = SingleToInt32{ .v2 = i }.v1;
				ret *= 1.5f - (x * 0.5f * ret * ret);
				return ret;
			}
			else
			{
				union DoubleToInt64
				{
					double v1;
					int64 v2;
				};

				int64 i = 0x5FE6EB50C7B537A9 - (DoubleToInt64{ .v1 = x }.v2 >> 1);
				double ret = DoubleToInt64{ .v2 = i }.v1;
				ret *= 1.5 - (x * 0.5 * ret * ret);
				return (T)ret;
			}
		}

		/// <summary>
		/// Get mod value.
		/// </summary>
		/// <typeparam name="T"> The type of floating point. </typeparam>
		/// <param name="x"> The argument value. </param>
		/// <param name="divisor"> The divisor. </param>
		/// <returns> The computed value. </returns>
		template<std::floating_point T>
		inline static constexpr T Mod(const T& x, const T& divisor)
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
		inline static constexpr T Mod(const T& x, const T& divisor) requires requires
		{
			{ x% divisor };
		}
		{
			return x % divisor;
		}

		template<class TL, class TR>
		static FORCEINLINE constexpr auto Min(const TL& Lhs, const TR& Rhs) noexcept
		{
			return std::min(Lhs, Rhs);
		}

		template<class TL, class TR>
		static FORCEINLINE constexpr auto Max(const TL& Lhs, const TR& Rhs) noexcept
		{
			return std::max(Lhs, Rhs);
		}

		template<class TL, class TR, class... T>
		inline static constexpr auto Min(const TL& Lhs, const TR& Rhs, const T&... Values)
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
		inline static constexpr auto Max(const TL& Lhs, const TR& Rhs, const T&... Values)
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
		inline static constexpr T Clamp(const T& value, const T& lhs, const T& rhs)
		{
			return Max(Min(value, rhs), lhs);
		}

		/// <summary>
		/// Determines whether a given floating-point value is within a specified inclusive range.
		/// </summary>
		/// <typeparam name="T">
		/// A floating-point type, such as float or double, used for comparison.
		/// </typeparam>
		/// <param name="v">
		/// The value to check if it falls within the range.
		/// </param>
		/// <param name="minv">
		/// The minimum value of the range, inclusive.
		/// </param>
		/// <param name="maxv">
		/// The maximum value of the range, inclusive.
		/// </param>
		/// <returns>
		/// Returns <c>true</c> if the value <paramref name="v"/> is greater than or equal to <paramref name="minv"/>
		/// and less than or equal to <paramref name="maxv"/>. Otherwise, returns <c>false</c>.
		/// </returns>
		template<std::floating_point T>
		inline static constexpr bool IsWithinInclusive(T v, T minv, T maxv)
		{
			return v >= minv && v <= maxv;
		}

		/// <summary>
		/// Rounds a given floating-point value to the nearest integer value.
		/// </summary>
		/// <typeparam name="T">
		/// A floating-point type, such as float or double, used for the rounding operation.
		/// </typeparam>
		/// <param name="v">
		/// The value to be rounded to the nearest integer.
		/// </param>
		/// <returns>
		/// Returns the rounded value of the input <paramref name="v"/> as a floating-point number of type <typeparamref name="T"/>.
		/// If the fractional part of <paramref name="v"/> is 0.5 or greater, it rounds up (away from zero). 
		/// Otherwise, it rounds down (toward zero).
		/// </returns>
		template<std::floating_point T>
		static constexpr T Round(const T& v)
		{
			if (v >= 0.0)
			{
				return (T)(int32)(v + 0.5);
			}
			else
			{
				return (T)(int32)(v - 0.5);
			}
		}

		/// <summary>
		/// Represents small number.
		/// </summary>
		inline static constexpr float SmallNumber = 0.0001f;

		template<EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InSine)
		{
			return sin(1.5707963 * t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutSine)
		{
			return 1 + sin(1.5707963 * (--t));
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutSine)
		{
			return 0.5 * (1 + sin(3.1415926 * (t - 0.5)));
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InQuad)
		{
			return t * t;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutQuad)
		{
			return t * (2 - t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutQuad)
		{
			return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InCubic)
		{
			return t * t * t;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutCubic)
		{
			return 1 + (--t) * t * t;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutCubic)
		{
			return t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InQuart)
		{
			t *= t;
			return t * t;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutQuart)
		{
			t = (--t) * t;
			return 1 - t * t;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutQuart)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InQuint)
		{
			double t2 = t * t;
			return t * t2 * t2;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutQuint)
		{
			double t2 = (--t) * t;
			return 1 + t * t2 * t2;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutQuint)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InExpo)
		{
			return (pow(2, 8 * t) - 1) / 255;
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutExpo)
		{
			return 1 - pow(2, -8 * t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutExpo)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InCirc)
		{
			return 1 - sqrt(1 - t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutCirc)
		{
			return sqrt(t);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutCirc)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InBack)
		{
			return t * t * (2.70158 * t - 1.70158);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutBack)
		{
			return 1 + (--t) * t * (2.70158 * t + 1.70158);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutBack)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InElastic)
		{
			double t2 = t * t;
			return t2 * t2 * sin(t * std::numbers::pi * 4.5);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutElastic)
		{
			double t2 = (t - 1) * (t - 1);
			return 1 - t2 * t2 * cos(t * std::numbers::pi * 4.5);
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutElastic)
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

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InBounce)
		{
			return pow(2, 6 * (t - 1)) * abs(sin(t * std::numbers::pi * 3.5));
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::OutBounce)
		{
			return 1 - pow(2, -6 * t) * abs(cos(t * std::numbers::pi * 3.5));
		}

		template<Ayla::EaseFunction Function>
		static double EaseFunction(double t) requires (Function == EaseFunction::InOutBounce)
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
		static T EaseFunction(Ayla::EaseFunction Function, T Value)
		{
#define EASE_CASE(Function) case EaseFunction::Function: return (T)EaseFunction<Ayla::EaseFunction::Function>(Value);
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

		inline static float Floor(float InValue) noexcept { return std::floor(InValue); }
		inline static double Floor(double InValue) noexcept { return std::floor(InValue); }
		inline static float Ceil(float InValue) noexcept { return std::ceil(InValue); }
		inline static double Ceil(double InValue) noexcept { return std::ceil(InValue); }
	};
}