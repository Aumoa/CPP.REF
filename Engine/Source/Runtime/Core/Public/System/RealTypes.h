// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "System/StaticClass.h"
#include <concepts>

class String;

class CORE_API RealTypes : public StaticClass
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
	static_assert(RealTypes::IsReal<T>());
	T Value;

	inline TRealType() noexcept
		: Value(0)
	{
	}

	inline constexpr TRealType(const T& Val) noexcept
		: Value(Val)
	{
	}

	inline constexpr TRealType(const TRealType& Val) noexcept
		: Value(Val.Value)
	{
	}

	String ToString() const;
};

using Single = TRealType<float>;
using Double = TRealType<double>;