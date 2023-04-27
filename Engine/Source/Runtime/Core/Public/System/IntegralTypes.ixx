// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IntegralTypes;

export import <concepts>;
export import :StaticClass;

export using int8 = signed char;
export using uint8 = unsigned char;
export using int16 = signed short;
export using uint16 = unsigned short;
export using int32 = signed long;
export using uint32 = unsigned long;
export using int64 = signed long long;
export using uint64 = unsigned long long;

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
};

export template<class T>
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

export using Int8 = TIntegralType<int8>;
export using UInt8 = TIntegralType<uint8>;
export using Int16 = TIntegralType<int16>;
export using UInt16 = TIntegralType<uint16>;
export using Int32 = TIntegralType<int32>;
export using UInt32 = TIntegralType<uint32>;
export using Int64 = TIntegralType<int64>;
export using UInt64 = TIntegralType<uint64>;