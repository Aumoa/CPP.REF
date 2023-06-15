// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.IO:FileAccessMode;

export import Core.System;

export enum class EFileAccessMode : uint32
{
	Read = 0x80000000L,
	Write = 0x40000000L,
	Append = 0x00000004L,
	All = 0x10000000L
};

export inline constexpr EFileAccessMode operator |(const EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	using T = std::underlying_type_t<EFileAccessMode>;
	return (EFileAccessMode)((T)Lhs | (T)Rhs);
}

export inline constexpr EFileAccessMode operator &(const EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	using T = std::underlying_type_t<EFileAccessMode>;
	return (EFileAccessMode)((T)Lhs & (T)Rhs);
}

export inline constexpr EFileAccessMode operator ^(const EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	using T = std::underlying_type_t<EFileAccessMode>;
	return (EFileAccessMode)((T)Lhs ^ (T)Rhs);
}

export inline constexpr EFileAccessMode& operator |=(EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	return Lhs = Lhs | Rhs;
}

export inline constexpr EFileAccessMode& operator &=(EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	return Lhs = Lhs & Rhs;
}

export inline constexpr EFileAccessMode& operator ^=(EFileAccessMode& Lhs, const EFileAccessMode& Rhs)
{
	return Lhs = Lhs ^ Rhs;
}

export inline constexpr EFileAccessMode operator ~(const EFileAccessMode& Lhs)
{
	using T = std::underlying_type_t<EFileAccessMode>;
	return (EFileAccessMode)(~(T)Lhs);
}
