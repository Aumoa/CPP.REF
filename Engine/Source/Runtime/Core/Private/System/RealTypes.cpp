// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:RealTypes;

export import :Std;
export import :Forward;
export import :StaticClass;

export class CORE_API RealTypes : public StaticClass
{
public:
	template<class T>
	static constexpr bool IsReal() noexcept
	{
		return std::same_as<T, float>
			|| std::same_as<T, double>;
	}
};

export template<class T>
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

export using Single = TRealType<float>;
export using Double = TRealType<double>;