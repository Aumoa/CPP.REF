// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core::Mathematics
{
	template<TIsRealType T>
	inline constexpr TDegrees<T> TDegrees<T>::Clamped_get() const
	{
		return Math::Mod(Value, AngleMax);
	}

	template<TIsRealType T>
	inline constexpr TDegrees<T> TDegrees<T>::Normalized_get() const
	{
		TDegrees clamped = Clamped;
		if (clamped.Value > AngleHalf)
		{
			return clamped.Value - AngleMax;
		}
		else
		{
			return clamped;
		}
	}

	template<TIsRealType T>
	template<TIsRealType O>
	inline constexpr TRadians<O> TDegrees<T>::ToRadians() const
	{
		return Value * InvPI;
	}

	template<TIsRealType T>
	inline constexpr TRadians<T> TRadians<T>::Clamped_get() const
	{
		return Math::Mod(Value, AngleMax);
	}

	template<TIsRealType T>
	inline constexpr TRadians<T> TRadians<T>::Normalized_get() const
	{
		TRadians clamped = Clamped;
		if (clamped.Value > AngleHalf)
		{
			return clamped.Value - AngleMax;
		}
		else
		{
			return clamped;
		}
	}

	template<TIsRealType T>
	template<TIsRealType O>
	inline constexpr TDegrees<O> TRadians<T>::ToDegrees() const
	{
		return static_cast<O>(Value) * static_cast<O>(Inv180);
	}

	template<class T>
	inline T Math::Lerp(const T& left, const T& right, double t)
	{
		if constexpr (VHasLerp<T>)
		{
			return T::Lerp(left, right, t);
		}
		else
		{
			return left + (right - left) * t;
		}
	}

	template<class T>
	inline T Math::Square(const T& value)
	{
		return value * value;
	}

	template<class TItem>
	inline static TRadians<TItem> Math::Atan2(TItem y, TItem x)
	{
		const TRadians<TItem> rad_r = std::atan2(y, x);
		return rad_r;
	}

	template<class TItem>
	inline static TRadians<TItem> Math::Asin(TItem value)
	{
		const TRadians<TItem> rad_r = std::asin(value);
		return rad_r;
	}

	template<TIsRealType T>
	inline static std::pair<T, T> Math::SinCos(TRadians<T> value)
	{
		T quotient = (InvPI<T> * (T)0.5) * value.Value;
		if (value.Value >= 0)
		{
			quotient = (T)((int32)(quotient + (T)0.5));
		}
		else
		{
			quotient = (T)((int32)(quotient - (T)0.5));
		}
		T y = value.Value - ((T)2.0 * PI<T>) * quotient;

		T sign;
		if (y > HalfPI<T>)
		{
			y = PI<T> - y;
			sign = (T)-1.0;
		}
		else if (y < -HalfPI<T>)
		{
			y = -PI<T> - y;
			sign = (T)-1.0;
		}
		else
		{
			sign = (T)+1.0;
		}

		T y2 = y * y;

		T sin = ((((((T)-2.3889859e-08 * y2 + (T)2.7525562e-06) * y2 - (T)0.00019840874) * y2 + (T)0.0083333310) * y2 - (T)0.16666667) * y2 + (T)1.0) * y;
		T p = (((((T)-2.6051615e-07 * y2 + (T)2.4760495e-05) * y2 - (T)0.0013888378) * y2 + (T)0.041666638) * y2 - (T)0.5) * y2 + (T)1.0;
		T cos = sign * p;

		return { sin, cos };
	}

	template<class T>
	inline static constexpr T Math::Mod(const T& value, const T& divisor)
	{
		T exp = value / divisor;
		int64 exp_n = (int64)exp;
		T re = exp - (T)exp_n;
		return divisor * re;
	}

	template<class T>
	inline static constexpr T Math::Abs(const T& value)
	{
		return value < (T)0 ? -value : value;
	}
}