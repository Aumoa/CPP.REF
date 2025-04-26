// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Math_.h"
#include "Radians.h"
#include "Degrees.h"

namespace Ayla
{
	template<std::floating_point T>
	inline constexpr bool Radians<T>::NearlyEquals(const Radians<T>& Rhs, const T& Epsilon) const
	{
		return Math::Abs(Value - Rhs.Value) <= Epsilon;
	}

	template<std::floating_point T>
	inline constexpr Radians<T> Radians<T>::GetClamped() const
	{
		return Math::Mod(Value, AngleMax);
	}

	template<std::floating_point T>
	inline constexpr Degrees<T> Radians<T>::ToDegrees() const
	{
		return Value * _180PI;
	}

	template<std::floating_point T>
	inline constexpr bool Degrees<T>::NearlyEquals(const Degrees<T>& Rhs, const T& Epsilon) const
	{
		return Math::Abs(Value - Rhs.Value) <= Epsilon;
	}

	template<std::floating_point T>
	inline constexpr Degrees<T> Degrees<T>::GetClamped() const
	{
		return Math::Mod(Value, AngleMax);
	}

	template<std::floating_point T>
	inline constexpr Radians<T> Degrees<T>::ToRadians() const
	{
		return Radians(Value * PI180);
	}

	template<std::floating_point T>
	inline T Math::Sin(const Radians<T>& v)
	{
		return sin(v.Value);
	}

	template<std::floating_point T>
	inline T Math::Cos(const Radians<T>& v)
	{
		return cos(v.Value);
	}


	template<std::floating_point T>
	inline T Math::Tan(const Radians<T>& v)
	{
		return tan(v.Value);
	}

	template<std::floating_point T>
	inline Radians<T> Math::Asin(const T& x)
	{
		return asin(x);
	}

	template<std::floating_point T>
	inline Radians<T> Math::Acos(const T& x)
	{
		return acos(x);
	}

	template<std::floating_point T>
	inline T Math::Atan(const T& x)
	{
		return atan(x);
	}

	template<std::floating_point T>
	inline T Math::Atan2(const T& y, const T& x)
	{
		return atan2(y, x);
	}

	template<std::floating_point T>
	constexpr void Math::SinCos(const Radians<T>& x, T& sin, T& cos)
	{
		static constexpr T pi = std::numbers::pi_v<T>;
		static constexpr T half_pi = pi * 0.5;
		static constexpr T inv_pi = std::numbers::inv_pi_v<T>;

		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		T quotient = (inv_pi * 0.5) * x.Value;
		if (x.Value >= 0.0)
		{
			quotient = (T)((int)(quotient + 0.5));
		}
		else
		{
			quotient = (T)((int)(quotient - 0.5));
		}
		T y = x.Value - (2.0 * pi) * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		T sign;
		if (y > half_pi)
		{
			y = pi - y;
			sign = -1.0;
		}
		else if (y < -half_pi)
		{
			y = -pi - y;
			sign = -1.0;
		}
		else
		{
			sign = +1.0;
		}

		T y2 = y * y;

		// 11-degree minimax approximation
		sin = (((((-2.3889859e-08 * y2 + 2.7525562e-06) * y2 - 0.00019840874) * y2 + 0.0083333310) * y2 - 0.16666667) * y2 + 1.0) * y;

		// 10-degree minimax approximation
		T p = ((((-2.6051615e-07 * y2 + 2.4760495e-05) * y2 - 0.0013888378) * y2 + 0.041666638) * y2 - 0.5) * y2 + 1.0;
		cos = sign * p;
	}
}
