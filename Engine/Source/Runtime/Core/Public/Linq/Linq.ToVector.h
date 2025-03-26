// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>
#include <vector>

namespace Ayla::inline Linq
{
	namespace Adaptors
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

			template<std::ranges::input_range R>
			friend inline constexpr auto operator |(R&& InRange, const Ayla::Linq::Adaptors::to_vector_adaptor_closure& Adaptor)
			{
				return Adaptor(std::forward<R>(InRange));
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
	}

	inline namespace Views
	{
		constexpr Adaptors::to_vector_adaptor ToVector;
	}
}