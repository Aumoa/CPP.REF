// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

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