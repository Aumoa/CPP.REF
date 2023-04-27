// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:StringSplitOptions;

export import <utility>;

export enum class EStringSplitOptions
{
	None = 0,
	RemoveEmptyEntries = 0x1,
	TrimEntries = 0x2,
};

export inline constexpr EStringSplitOptions operator |(EStringSplitOptions Lhs, EStringSplitOptions Rhs) noexcept
{
	using T = std::underlying_type_t<EStringSplitOptions>;
	return (EStringSplitOptions)((T)Lhs | (T)Rhs);
}

export inline constexpr EStringSplitOptions operator &(EStringSplitOptions Lhs, EStringSplitOptions Rhs) noexcept
{
	using T = std::underlying_type_t<EStringSplitOptions>;
	return (EStringSplitOptions)((T)Lhs & (T)Rhs);
}

export inline constexpr EStringSplitOptions& operator |=(EStringSplitOptions& Lhs, EStringSplitOptions Rhs) noexcept
{
	Lhs = Lhs | Rhs;
	return Lhs;
}

export inline constexpr EStringSplitOptions& operator &=(EStringSplitOptions& Lhs, EStringSplitOptions Rhs) noexcept
{
	Lhs = Lhs & Rhs;
	return Lhs;
}

export inline constexpr EStringSplitOptions operator ~(EStringSplitOptions u) noexcept
{
	using T = std::underlying_type_t<EStringSplitOptions>;
	return (EStringSplitOptions)~(T)u;
}