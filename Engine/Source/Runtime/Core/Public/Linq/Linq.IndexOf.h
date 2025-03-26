// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Ayla::inline Linq
{
	namespace Adaptors
	{
		template<class T>
		struct index_of_adaptor_closure
		{
			const T& Comparand;

			inline constexpr index_of_adaptor_closure(const T& InComparand) noexcept
				: Comparand(InComparand)
			{
			}

			template<std::ranges::input_range R> requires std::ranges::sized_range<R>
			inline constexpr auto operator ()(const R& InRange) const noexcept(noexcept(*std::ranges::begin(InRange) == Comparand))
			{
				size_t Index = 0;
				for (auto& V : InRange)
				{
					if (V == Comparand)
					{
						return Index;
					}
					++Index;
				}
				return static_cast<size_t>(-1);
			}

			template<std::ranges::input_range R, class T>
			friend constexpr auto operator |(const R& InRange, const Ayla::Linq::Adaptors::index_of_adaptor_closure<T>& Adaptor) noexcept(noexcept(Adaptor(InRange)))
			{
				return Adaptor(InRange);
			}
		};

		template<class T>
		index_of_adaptor_closure(const T&) -> index_of_adaptor_closure<T>;

		struct index_of_adaptor
		{
			template<class T>
			inline constexpr auto operator ()(const T& InComparand) const noexcept
			{
				return index_of_adaptor_closure(InComparand);
			}

			template<std::ranges::input_range R, class T>
			inline constexpr auto operator ()(const R& InRange, const T& InComparand) const noexcept(noexcept(operator ()(InComparand)(InRange)))
			{
				return operator ()(InComparand)(InRange);
			}
		};
	}

	inline namespace Views
	{
		constexpr Adaptors::index_of_adaptor IndexOf;
	}
}