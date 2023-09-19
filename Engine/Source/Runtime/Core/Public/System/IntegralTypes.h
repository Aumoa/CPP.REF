// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "System/StaticClass.h"
#include "Platform/PlatformTypes.h"

using int8 = PlatformTypes::int8;
using uint8 = PlatformTypes::uint8;
using int16 = PlatformTypes::int16;
using uint16 = PlatformTypes::uint16;
using int32 = PlatformTypes::int32;
using uint32 = PlatformTypes::uint32;
using int64 = PlatformTypes::int64;
using uint64 = PlatformTypes::uint64;
using byte = PlatformTypes::byte;

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

	static constexpr size_t Npos = ~size_t(0);
};

template<class T>
struct CORE_API TIntegralType
{
	using UnderlyingType = T;
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

	static String ToString(const T& InValue);
	static bool TryParse(String InStr, T& OutValue, int32 Base = 10) noexcept;
};

using Int8 = TIntegralType<int8>;
using UInt8 = TIntegralType<uint8>;
using Int16 = TIntegralType<int16>;
using UInt16 = TIntegralType<uint16>;
using Int32 = TIntegralType<int32>;
using UInt32 = TIntegralType<uint32>;
using Int64 = TIntegralType<int64>;
using UInt64 = TIntegralType<uint64>;