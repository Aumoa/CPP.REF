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
			inline constexpr auto operator ()(R&& InRange) const noexcept(noexcept(std::is_rvalue_reference_v<R>))
			{
				using T = std::ranges::range_value_t<R>;

				std::vector<T> Output;
				if constexpr (std::ranges::sized_range<R>)
				{
					size_t RangeSize = std::ranges::size(InRange);
					Output.reserve(RangeSize);
				}
				for (auto& Elem : InRange)
				{
					if constexpr (std::is_rvalue_reference_v<R>)
					{
						Output.emplace_back(std::move(Elem));
					}
					else
					{
						Output.emplace_back(Elem);
					}
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
			inline constexpr auto operator ()(R&& InRange) const noexcept(noexcept(operator ()(std::forward<R>(InRange))))
			{
				return operator ()()(std::forward<R>(InRange));
			}
		};

		template<std::ranges::input_range R>
		inline constexpr auto operator |(R&& InRange, const to_vector_adaptor_closure& Adaptor) noexcept(std::is_rvalue_reference_v<R>)
		{
			return Adaptor(std::forward<R>(InRange));
		}
	}

	inline namespace views
	{
		constexpr details::to_vector_adaptor ToVector;
	}
}