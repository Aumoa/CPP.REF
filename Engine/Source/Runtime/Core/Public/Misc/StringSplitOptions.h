// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>

namespace libty::inline Core
{
	enum class EStringSplitOptions
	{
		None = 0,
		RemoveEmptyEntries = 0x1,
		TrimEntries = 0x2,
	};

	inline constexpr EStringSplitOptions operator |(EStringSplitOptions lhs, EStringSplitOptions rhs) noexcept
	{
		using T = std::underlying_type_t<EStringSplitOptions>;
		return (EStringSplitOptions)((T)lhs | (T)rhs);
	}

	inline constexpr EStringSplitOptions operator &(EStringSplitOptions lhs, EStringSplitOptions rhs) noexcept
	{
		using T = std::underlying_type_t<EStringSplitOptions>;
		return (EStringSplitOptions)((T)lhs & (T)rhs);
	}

	inline constexpr EStringSplitOptions& operator |=(EStringSplitOptions& lhs, EStringSplitOptions rhs) noexcept
	{
		lhs = lhs | rhs;
		return lhs;
	}

	inline constexpr EStringSplitOptions& operator &=(EStringSplitOptions& lhs, EStringSplitOptions rhs) noexcept
	{
		lhs = lhs & rhs;
		return lhs;
	}

	inline constexpr EStringSplitOptions operator ~(EStringSplitOptions u) noexcept
	{
		using T = std::underlying_type_t<EStringSplitOptions>;
		return (EStringSplitOptions)~(T)u;
	}
}