// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "Misc/StaticClass.h"

using int8 = signed char;
using uint8 = unsigned char;
using int16 = signed short;
using uint16 = unsigned short;
using int32 = signed long;
using uint32 = unsigned long;
using int64 = signed long long;
using uint64 = unsigned long long;

class String;

class CORE_API IntegralTypes : public StaticClass
{
public:
	template<class T>
	static constexpr bool IsIntegral() noexcept
	{
		return std::same_as<T, int8>
			|| std::same_as<T, uint8>
			|| std::same_as<T, int16>
			|| std::same_as<T, uint16>
			|| std::same_as<T, int32>
			|| std::same_as<T, uint32>
			|| std::same_as<T, int64>
			|| std::same_as<T, uint64>;
	}
};

template<class T>
struct TIntegralType
{
	static_assert(IntegralTypes::IsIntegral<T>());
	T Value;

	inline TIntegralType() noexcept
		: Value(0)
	{
	}

	inline constexpr TIntegralType(const T& Val) noexcept
		: Value(Val)
	{
	}

	inline constexpr TIntegralType(const TIntegralType& Val) noexcept
		: Value(Val.Value)
	{
	}

    String ToString() const;
};

using Int8 = TIntegralType<int8>;
using UInt8 = TIntegralType<uint8>;
using Int16 = TIntegralType<int16>;
using UInt16 = TIntegralType<uint16>;
using Int32 = TIntegralType<int32>;
using UInt32 = TIntegralType<uint32>;
using Int64 = TIntegralType<int64>;
using UInt64 = TIntegralType<uint64>;