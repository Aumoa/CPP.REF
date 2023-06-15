// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.IO:FileSharedMode;

export import <utility>;

export enum class EFileSharedMode
{
	None = 0,
	Read = 0x1,
	Write = 0x2,
	Delete = 0x4
};

export inline constexpr EFileSharedMode operator |(const EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	using T = std::underlying_type_t<EFileSharedMode>;
	return (EFileSharedMode)((T)Lhs | (T)Rhs);
}

export inline constexpr EFileSharedMode operator &(const EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	using T = std::underlying_type_t<EFileSharedMode>;
	return (EFileSharedMode)((T)Lhs & (T)Rhs);
}

export inline constexpr EFileSharedMode operator ^(const EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	using T = std::underlying_type_t<EFileSharedMode>;
	return (EFileSharedMode)((T)Lhs ^ (T)Rhs);
}

export inline constexpr EFileSharedMode& operator |=(EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	return Lhs = Lhs | Rhs;
}

export inline constexpr EFileSharedMode& operator &=(EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	return Lhs = Lhs & Rhs;
}

export inline constexpr EFileSharedMode& operator ^=(EFileSharedMode& Lhs, const EFileSharedMode& Rhs)
{
	return Lhs = Lhs ^ Rhs;
}

export inline constexpr EFileSharedMode operator ~(const EFileSharedMode& Lhs)
{
	using T = std::underlying_type_t<EFileSharedMode>;
	return (EFileSharedMode)(~(T)Lhs);
}
