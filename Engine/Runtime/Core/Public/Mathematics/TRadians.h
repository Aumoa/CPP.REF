// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

template<TIsRealType T = float>
struct TRadians
{
private:
	static constexpr T PI = (T)3.141592;
	static constexpr T Inv180 = (T)180.0 / PI;
	static constexpr T AngleMax = PI * (T)2;
	static constexpr T AngleHalf = PI;

public:
	T Value;

	inline TRadians()
	{

	}

	inline constexpr TRadians(const T& value) : Value(value)
	{

	}

	inline constexpr TRadians(const TRadians& value) : Value(value.Value)
	{

	}

	inline TRefPtr<String> ToString() const
	{
		return String::Format(L"{0}rad", Value);
	}

	vs_property_get(TRadians, Clamped);
	inline constexpr TRadians Clamped_get() const;

	vs_property_get(TRadians, Normalized);
	inline constexpr TRadians Normalized_get() const;

	template<TIsRealType O = T>
	inline constexpr TDegrees<O> ToDegrees() const;

	inline TRadians& operator =(const TRadians& value)
	{
		Value = value.Value;
		return *this;
	}

	inline TRadians& operator +=(const TRadians& value)
	{
		Value += value.Value;
		return *this;
	}

	inline TRadians& operator -=(const TRadians& value)
	{
		Value -= value.Value;
		return *this;
	}

	inline TRadians& operator *=(const T& value)
	{
		Value *= value;
		return *this;
	}

	inline TRadians& operator /=(const T& value)
	{
		Value /= value;
		return *this;
	}

	inline constexpr TRadians operator -() const
	{
		return TRadians(-Value);
	}

	inline constexpr TRadians operator +(const TRadians& value) const
	{
		return TRadians(Value + value.Value);
	}

	inline constexpr TRadians operator -(const TRadians& value) const
	{
		return TRadians(Value - value.Value);
	}

	inline constexpr TRadians operator *(const T& value) const
	{
		return TRadians(Value * value);
	}

	inline constexpr TRadians operator /(const T& value) const
	{
		return TRadians(Value / value);
	}

	template<TIsRealType O> requires TIsSmallThen<O, T>
	inline constexpr explicit operator TRadians<O>() const
	{
		return TRadians<O>(static_cast<O>(Value));
	}

	template<TIsRealType O> requires TIsGreaterEqualsThen<O, T>
	inline constexpr operator TRadians<O>() const
	{
		return TRadians<O>(static_cast<O>(Value));
	}

	template<TIsRealType O> requires TIsSmallThen<O, T>
	inline constexpr explicit operator TDegrees<O>() const
	{
		return ToDegrees<O>();
	}

	template<TIsRealType O> requires TIsGreaterEqualsThen<O, T>
	inline constexpr operator TDegrees<O>() const
	{
		return ToDegrees<O>();
	}

	inline constexpr bool operator ==(const TRadians& rh) const
	{
		return Value == rh.Value;
	}

	inline constexpr bool operator !=(const TRadians& rh) const
	{
		return Value != rh.Value;
	}

	inline constexpr bool operator <(const TRadians& rh) const
	{
		return Value < rh.Value;
	}

	inline constexpr bool operator >(const TRadians& rh) const
	{
		return Value > rh.Value;
	}

	inline constexpr bool operator <=(const TRadians& rh) const
	{
		return Value <= rh.Value;
	}

	inline constexpr bool operator >=(const TRadians& rh) const
	{
		return Value >= rh.Value;
	}
};

template<TIsRealType T>
inline constexpr TRadians<T> operator *(const T& lh, const TRadians<T>& rh)
{
	return TRadians<T>(lh * rh.Value);
}

template<TIsRealType T>
inline constexpr TRadians<T> operator /(const T& lh, const TRadians<T>& rh)
{
	return TRadians<T>(lh / rh.Value);
}

inline constexpr TRadians<long double> operator "" _rad(long double value)
{
	return value;
}