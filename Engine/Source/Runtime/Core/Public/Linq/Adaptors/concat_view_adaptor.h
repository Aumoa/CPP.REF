// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "Linq/Ranges/concat_view.h"

namespace Ayla::inline Linq::Adaptors
{
	template<std::ranges::input_range R>
	struct concat_view_adaptor_closure
	{
		R r2;

		template<std::ranges::input_range U>
		constexpr concat_view_adaptor_closure(U&& r2) noexcept
			: r2(std::forward<U>(r2))
		{
		}

		template<std::ranges::input_range U>
		constexpr auto operator ()(U&& r1) noexcept
		{
			return ranges::concat_view(std::forward<U>(r1), std::forward<R>(r2));
		}

		template<std::ranges::input_range R1, std::ranges::input_range R2>
		friend constexpr auto operator |(R1&& r1, Ayla::Linq::Adaptors::concat_view_adaptor_closure<R2>&& adaptor) noexcept
		{
			return adaptor(std::forward<R1>(r1));
		}
	};

	template<std::ranges::input_range R>
	concat_view_adaptor_closure(R&&) -> concat_view_adaptor_closure<R>;

	struct concat_view_adaptor
	{
		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& r2) const noexcept
		{
			return concat_view_adaptor_closure(std::forward<R>(r2));
		}
	};
}