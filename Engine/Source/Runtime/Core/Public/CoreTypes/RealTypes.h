// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "Misc/StaticClass.h"

class CORE_API NRealTypes : public StaticClass
{
public:
	template<class T>
	static constexpr bool IsReal() noexcept
	{
		return std::same_as<T, float>
			|| std::same_as<T, double>;
	}
};

template<class T>
struct TRealType
{
	static_assert(NRealTypes::IsReal<T>);
	T Value;

	inline TRealType() noexcept
		: Value(0)
	{
	}

	constexpr TRealType(const T& Val) noexcept
		: Value(Val)
	{
	}

	constexpr TRealType(const TRealType& Val) noexcept
		: Value(Val.Value)
	{
	}
};

using Single = TRealType<float>;
using Double = TRealType<double>;