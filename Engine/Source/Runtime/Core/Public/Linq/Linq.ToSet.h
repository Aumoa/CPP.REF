// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>
#include <set>

namespace Linq
{
	namespace details
	{
		struct to_set_adaptor_closure
		{
			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const
			{
				using T = std::ranges::range_value_t<R>;

				std::set<T> Output;
				for (const auto& Elem : InRange)
				{
					Output.emplace(Elem);
				}
				return Output;
			}
		};

		struct to_set_adaptor
		{
			inline constexpr auto operator ()() const noexcept
			{
				return to_set_adaptor_closure();
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const
			{
				return operator ()()(InRange);
			}
		};

		template<std::ranges::input_range R>
		inline constexpr auto operator |(const R& InRange, const to_set_adaptor_closure& Adaptor)
		{
			return Adaptor(InRange);
		}
	}

	inline namespace views
	{
		constexpr details::to_set_adaptor ToSet;
	}
}