// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Ayla::inline Linq
{
	namespace Adaptors
	{
		struct is_valid_index_adaptor_closure
		{
			const size_t Index;

			inline constexpr is_valid_index_adaptor_closure(size_t InIndex) noexcept
				: Index(InIndex)
			{
			}

			template<std::ranges::sized_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
			{
				return std::ranges::size(InRange) > Index;
			}

			template<std::ranges::sized_range R>
			friend inline constexpr auto operator |(const R& InRange, const Ayla::Linq::Adaptors::is_valid_index_adaptor_closure& Adaptor) noexcept
			{
				return Adaptor(InRange);
			}
		};

		struct is_valid_index_adaptor
		{
			inline constexpr auto operator ()(size_t Index) const noexcept
			{
				return is_valid_index_adaptor_closure(Index);
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange, size_t Index) const noexcept
			{
				return operator ()(Index)(InRange);
			}
		};
	}

	inline namespace Views
	{
		constexpr Adaptors::is_valid_index_adaptor IsValidIndex;
	}
}