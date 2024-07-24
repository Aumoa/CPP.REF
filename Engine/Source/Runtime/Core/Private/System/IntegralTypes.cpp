// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:IntegralTypes;

export import :Std;
export import :StaticClass;
export import :PlatformTypes;

export using int8 = PlatformTypes::int8;
export using uint8 = PlatformTypes::uint8;
export using int16 = PlatformTypes::int16;
export using uint16 = PlatformTypes::uint16;
export using int32 = PlatformTypes::int32;
export using uint32 = PlatformTypes::uint32;
export using int64 = PlatformTypes::int64;
export using uint64 = PlatformTypes::uint64;
export using byte = PlatformTypes::byte;

export class String;

export class CORE_API IntegralTypes : public StaticClass
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

export template<class T>
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

export using Int8 = TIntegralType<int8>;
export using UInt8 = TIntegralType<uint8>;
export using Int16 = TIntegralType<int16>;
export using UInt16 = TIntegralType<uint16>;
export using Int32 = TIntegralType<int32>;
export using UInt32 = TIntegralType<uint32>;
export using Int64 = TIntegralType<int64>;
export using UInt64 = TIntegralType<uint64>;