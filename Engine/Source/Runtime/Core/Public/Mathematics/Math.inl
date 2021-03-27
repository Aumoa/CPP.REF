// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T>
inline constexpr T Math::Lerp(const T& left, const T& right, float t)
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
inline constexpr T Math::Square(const T& value)
{
	return value * value;
}

template<class T>
inline constexpr T Math::Mod(const T& value, const T& divisor)
{
	T exp = value / divisor;
	int64 exp_n = (int64)exp;
	T re = exp - (T)exp_n;
	return divisor * re;
}

template<class T>
inline constexpr T Math::Abs(const T& value)
{
	return value < (T)0 ? -value : value;
}

template<class T, class... TRest>
inline constexpr T Math::Min(const T& lh, const T& rh, TRest&&... rhs)
{
	if constexpr (sizeof...(TRest) == 0)
	{
		return lh < rh ? lh : rh;
	}
	else
	{
		auto r = Min(rh, std::forward<TRest...>(rhs)...);
		return lh < r ? lh : r;
	}
}

template<TOverrideMinMax T, class... TRest>
inline constexpr T Math::Min(const T& lh, const T& rh, TRest&&... rhs)
{
	if constexpr (sizeof...(TRest) == 0)
	{
		return T::Min(lh, rh);
	}
	else
	{
		auto r = Min(rh, std::forward<TRest...>(rhs)...);
		return T::Min(lh, r);
	}
}

template<class T, class... TRest>
inline constexpr T Math::Max(const T& lh, const T& rh, TRest&&... rhs)
{
	if constexpr (sizeof...(TRest) == 0)
	{
		return lh > rh ? lh : rh;
	}
	else
	{
		auto r = Min(rh, std::forward<TRest...>(rhs)...);
		return lh > r ? lh : r;
	}
}

template<TOverrideMinMax T, class... TRest>
inline constexpr T Math::Max(const T& lh, const T& rh, TRest&&... rhs)
{
	if constexpr (sizeof...(TRest) == 0)
	{
		return T::Max(lh, rh);
	}
	else
	{
		auto r = Min(rh, std::forward<TRest...>(rhs)...);
		return T::Max(lh, r);
	}
}

template<class T>
inline constexpr T Math::Clamp(const T& value, const T& start, const T& end)
{
	return Max(Min(value, end), start);
}

template<class T>
inline constexpr bool Math::InRange(const T& value, const T& start, const T& end)
{
	return value >= start && value <= end;
}