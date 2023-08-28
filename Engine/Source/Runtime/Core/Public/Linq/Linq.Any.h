// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Linq
{
	namespace details
	{
		struct any_adaptor_closure
		{
			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
			{
				if constexpr (std::ranges::sized_range<R>)
				{
					return std::ranges::size(InRange) > 0;
				}
				else
				{
					return std::ranges::empty(InRange) == false;
				}
			}
		};

		struct any_adaptor
		{
			inline constexpr auto operator ()() const noexcept
			{
				return any_adaptor_closure();
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
			{
				return any_adaptor_closure()(InRange);
			}
		};

		template<std::ranges::input_range R>
		inline constexpr auto operator |(const R& InRange, const any_adaptor_closure& Adaptor) noexcept
		{
			return Adaptor(InRange);
		}
	}

	inline namespace views
	{
		constexpr details::any_adaptor Any;
	}
}