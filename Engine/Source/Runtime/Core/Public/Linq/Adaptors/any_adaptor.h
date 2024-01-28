// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>

namespace Linq::adaptors
{
	struct any_adaptor_closure
	{
		template<std::ranges::input_range R>
		inline constexpr auto operator ()(R&& view) const noexcept
		{
			if constexpr (std::ranges::sized_range<R>)
			{
				return std::ranges::size(std::forward<R>(view)) > 0;
			}
			else
			{
				return std::ranges::empty(std::forward<R>(view)) == false;
			}
		}
	};

	struct any_adaptor
	{
		inline constexpr auto operator ()() const noexcept
		{
			return any_adaptor_closure();
		}
	};

	template<std::ranges::input_range R>
	inline constexpr auto operator |(R&& view, any_adaptor_closure&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}
}