// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Ayla::inline Linq
{
	namespace Adaptors
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

			template<std::ranges::input_range R>
			friend inline constexpr auto operator |(const R& InRange, const Ayla::Linq::Adaptors::to_set_adaptor_closure& Adaptor)
			{
				return Adaptor(InRange);
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
	}

	inline namespace Views
	{
		constexpr Adaptors::to_set_adaptor ToSet;
	}
}