// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>
#include <vector>

namespace Linq
{
	namespace details
	{
		struct to_vector_adaptor_closure
		{
			template<std::ranges::input_range R>
			inline constexpr auto operator ()(R&& InRange) const
			{
				using T = std::ranges::range_value_t<R>;

				std::vector<T> Output;
				if constexpr (std::ranges::sized_range<R>)
				{
					size_t RangeSize = std::ranges::size(InRange);
					Output.reserve(RangeSize);
				}
				for (const auto& Elem : InRange)
				{
					Output.emplace_back(Elem);
				}
				return Output;
			}
		};

		struct to_vector_adaptor
		{
			inline constexpr auto operator ()() const noexcept
			{
				return to_vector_adaptor_closure();
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(R&& InRange) const
			{
				return operator ()()(std::forward<R>(InRange));
			}
		};

		template<std::ranges::input_range R>
		inline constexpr auto operator |(R&& InRange, const to_vector_adaptor_closure& Adaptor)
		{
			return Adaptor(std::forward<R>(InRange));
		}
	}

	inline namespace views
	{
		constexpr details::to_vector_adaptor ToVector;
	}
}