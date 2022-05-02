// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Console
{
	SENUM(CONSOLE_API, EConsoleModifiers, int32,
		None, 0,
		Alt, 0x1,
		Shift, 0x2,
		Control, 0x4
	);

	template<class TConsoleModifiersL, class TConsoleModifiersR>
	constexpr EConsoleModifiers operator |(const TConsoleModifiersL& lhs, const TConsoleModifiersR& rhs) requires
		std::same_as<TConsoleModifiersL, EConsoleModifiers> ||
		std::same_as<TConsoleModifiersL, EConsoleModifiers::__Tag__> ||
		std::same_as<TConsoleModifiersR, EConsoleModifiers> ||
		std::same_as<TConsoleModifiersR, EConsoleModifiers::__Tag__>
	{
		return (EConsoleModifiers)((int32)lhs | (int32)rhs);
	}

	template<class TConsoleModifiersL, class TConsoleModifiersR>
	constexpr EConsoleModifiers operator &(const TConsoleModifiersL& lhs, const TConsoleModifiersR& rhs) requires
		std::same_as<TConsoleModifiersL, EConsoleModifiers> ||
		std::same_as<TConsoleModifiersL, EConsoleModifiers::__Tag__> ||
		std::same_as<TConsoleModifiersR, EConsoleModifiers> ||
		std::same_as<TConsoleModifiersR, EConsoleModifiers::__Tag__>
	{
		return (EConsoleModifiers)((int32)lhs & (int32)rhs);
	}

	template<class TConsoleModifiers>
	constexpr EConsoleModifiers& operator |=(EConsoleModifiers& lhs, const TConsoleModifiers& rhs) requires
		std::same_as<TConsoleModifiers, EConsoleModifiers> ||
		std::same_as<TConsoleModifiers, EConsoleModifiers::__Tag__>
	{
		return lhs = (EConsoleModifiers)((int32)lhs | (int32)rhs);
	}

	template<class TConsoleModifiers>
	constexpr EConsoleModifiers& operator &=(EConsoleModifiers& lhs, const TConsoleModifiers& rhs) requires
		std::same_as<TConsoleModifiers, EConsoleModifiers> ||
		std::same_as<TConsoleModifiers, EConsoleModifiers::__Tag__>
	{
		return lhs = (EConsoleModifiers)((int32)lhs & (int32)rhs);
	}

	template<class TConsoleModifiers>
	constexpr EConsoleModifiers operator ~(TConsoleModifiers lhs) requires
		std::same_as<TConsoleModifiers, EConsoleModifiers> ||
		std::same_as<TConsoleModifiers, EConsoleModifiers::__Tag__>
	{
		return (EConsoleModifiers)~(int32)lhs;
	}
}