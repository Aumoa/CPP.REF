// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Linq
{
	namespace details
	{
		template<class T>
		struct contains_adaptor_closure
		{
			const T& Comparand;

			inline constexpr contains_adaptor_closure(const T& InComparand) noexcept
				: Comparand(InComparand)
			{
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
				requires std::convertible_to<std::invoke_result_t<T, std::ranges::range_value_t<R>>, bool>
			{
				for (auto& V : InRange)
				{
					if (Comparand(V))
					{
						return true;
					}
				}
				return false;
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
				requires requires { { std::declval<const std::ranges::range_value_t<R>&>() == std::declval<const T&>() }; }
			{
				for (const auto& V : InRange)
				{
					if (V == Comparand)
					{
						return true;
					}
				}
				return false;
			}
		};

		template<class T>
		contains_adaptor_closure(const T&) -> contains_adaptor_closure<T>;

		struct contains_adaptor
		{
			template<class T>
			inline constexpr auto operator ()(const T& InComparand) const noexcept
			{
				return contains_adaptor_closure(InComparand);
			}

			template<std::ranges::input_range R, class T>
			inline constexpr auto operator ()(const R& InRange, const T& InComparand) const noexcept(noexcept(operator ()(InComparand)(InRange)))
			{
				return operator ()(InComparand)(InRange);
			}
		};

		template<std::ranges::input_range R, class T>
		constexpr auto operator |(const R& InRange, const contains_adaptor_closure<T>& Adaptor) noexcept(noexcept(Adaptor(InRange)))
		{
			return Adaptor(InRange);
		}
	}

	inline namespace views
	{
		constexpr details::contains_adaptor Contains;
	}
}