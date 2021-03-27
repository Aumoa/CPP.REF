// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"
#include "Core/String.h"

#include "Math.h"

template<TIsRealType T>
struct TRadians;

template<TIsRealType T = float>
struct TDegrees
{
private:
	static constexpr T PI = (T)3.141592;
	static constexpr T InvPI = PI / (T)180.0;
	static constexpr T AngleMax = (T)360.0;
	static constexpr T AngleHalf = (T)180.0;

public:
	T Value;

	inline TDegrees()
	{

	}

	inline constexpr TDegrees(const T& value) : Value(value)
	{

	}

	inline constexpr TDegrees(const TDegrees& value) : Value(value.Value)
	{

	}

	inline TRefPtr<String> ToString() const
	{
		return String::Format(L"{0}°", Value);
	}

	__declspec(property(get = Clamped_get)) TDegrees Clamped;
	[[nodiscard]] inline constexpr TDegrees Clamped_get() const;

	__declspec(property(get = Normalized_get)) TDegrees Normalized;
	[[nodiscard]] inline constexpr TDegrees Normalized_get() const;

	template<TIsRealType O = T>
	inline constexpr TRadians<O> ToRadians() const;

	inline TDegrees& operator =(const TDegrees& value)
	{
		Value = value.Value;
		return *this;
	}

	inline TDegrees& operator +=(const TDegrees& value)
	{
		Value += value.Value;
		return *this;
	}

	inline TDegrees& operator -=(const TDegrees& value)
	{
		Value -= value.Value;
		return *this;
	}

	inline TDegrees& operator *=(const T& value)
	{
		Value *= value;
		return *this;
	}

	inline TDegrees& operator /=(const T& value)
	{
		Value /= value;
		return *this;
	}

	inline constexpr TDegrees operator -() const
	{
		return TDegrees(-Value);
	}

	inline constexpr TDegrees operator +(const TDegrees& value) const
	{
		return TDegrees(Value + value.Value);
	}

	inline constexpr TDegrees operator -(const TDegrees& value) const
	{
		return TDegrees(Value - value.Value);
	}

	inline constexpr TDegrees operator *(const T& value) const
	{
		return TDegrees(Value * value);
	}

	inline constexpr TDegrees operator /(const T& value) const
	{
		return TDegrees(Value / value);
	}

	template<TIsRealType O> requires TIsSmallThen<O, T>
	inline constexpr explicit operator TDegrees<O>() const
	{
		return TDegrees<O>(static_cast<O>(Value));
	}

	template<TIsRealType O> requires TIsGreaterEqualsThen<O, T>
	inline constexpr operator TDegrees<O>() const
	{
		return TDegrees<O>(static_cast<O>(Value));
	}

	template<TIsRealType O> requires TIsSmallThen<O, T>
	inline constexpr explicit operator TRadians<O>() const
	{
		return ToRadians<O>();
	}

	template<TIsRealType O> requires TIsGreaterEqualsThen<O, T>
	inline constexpr operator TRadians<O>() const
	{
		return ToRadians<O>();
	}

	inline constexpr bool operator ==(const TDegrees& rh) const
	{
		return Value == rh.Value;
	}

	inline constexpr bool operator !=(const TDegrees& rh) const
	{
		return Value != rh.Value;
	}

	inline constexpr bool operator <(const TDegrees& rh) const
	{
		return Value < rh.Value;
	}

	inline constexpr bool operator >(const TDegrees& rh) const
	{
		return Value > rh.Value;
	}

	inline constexpr bool operator <=(const TDegrees& rh) const
	{
		return Value <= rh.Value;
	}

	inline constexpr bool operator >=(const TDegrees& rh) const
	{
		return Value >= rh.Value;
	}
};

template<TIsRealType T>
inline constexpr TDegrees<T> operator *(const T& lh, const TDegrees<T>& rh)
{
	return TDegrees<T>(lh * rh.Value);
}

template<TIsRealType T>
inline constexpr TDegrees<T> operator /(const T& lh, const TDegrees<T>& rh)
{
	return TDegrees<T>(lh / rh.Value);
}

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

	__declspec(property(get = Clamped_get)) TRadians Clamped;
	[[nodiscard]] inline constexpr TRadians Clamped_get() const;

	__declspec(property(get = Normalized_get)) TRadians Normalized;
	[[nodiscard]] inline constexpr TRadians Normalized_get() const;

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

using Radians = TRadians<float>;
using Degrees = TDegrees<float>;

inline constexpr Degrees operator"" _deg(long double value)
{
	return (float)value;
}

inline constexpr Radians operator "" _rad(long double value)
{
	return (float)value;
}

#include "TAngleDef.inl"