// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "Linq/Details/linked_iterator.h"

namespace Ayla::inline Linq::inline Ranges
{
	template<std::ranges::input_range R1, std::ranges::input_range R2> requires std::constructible_from<
		details::linked_iterator<
			std::ranges::iterator_t<R1>,
			std::ranges::iterator_t<R2>
		>,
		std::ranges::iterator_t<R1>,
		std::ranges::iterator_t<R1>,
		std::ranges::iterator_t<R2>,
		std::ranges::iterator_t<R2>
	>
	struct concat_view : public std::ranges::view_interface<concat_view<R1, R2>>
	{
	public:
		using iterator = details::linked_iterator<std::ranges::iterator_t<R1>, std::ranges::iterator_t<R2>>;

	private:
		R1 r1;
		R2 r2;

	public:
		template<std::ranges::input_range U1, std::ranges::input_range U2>
			requires std::constructible_from<
				iterator,
				std::ranges::iterator_t<U1>,
				std::ranges::iterator_t<U1>,
				std::ranges::iterator_t<U2>,
				std::ranges::iterator_t<U2>
			>
		constexpr concat_view(U1&& r1, U2&& r2) noexcept
			: r1(std::forward<U1>(r1))
			, r2(std::forward<U2>(r2))
		{
		}

		constexpr auto begin() noexcept
		{
			return iterator(std::ranges::begin(r1), std::ranges::end(r1), std::ranges::begin(r2), std::ranges::end(r2));
		}

		constexpr auto end() noexcept
		{
			return iterator(std::ranges::end(r1), std::ranges::end(r1), std::ranges::end(r2), std::ranges::end(r2));
		}

		constexpr auto begin() const noexcept
		{
			return iterator(std::ranges::cbegin(r1), std::ranges::cend(r1), std::ranges::cbegin(r2), std::ranges::cend(r2));
		}

		constexpr auto end() const noexcept
		{
			return iterator(std::ranges::cend(r1), std::ranges::cend(r1), std::ranges::cend(r2), std::ranges::cend(r2));
		}

		constexpr size_t size() const noexcept requires std::ranges::sized_range<R1> && std::ranges::sized_range<R2>
		{
			return std::ranges::size(r1) + std::ranges::size(r2);
		}
	};

	template<std::ranges::input_range R1, std::ranges::input_range R2> requires std::constructible_from<
		details::linked_iterator<
			std::ranges::iterator_t<R1>,
			std::ranges::iterator_t<R2>
		>,
		std::ranges::iterator_t<R1>,
		std::ranges::iterator_t<R1>,
		std::ranges::iterator_t<R2>,
		std::ranges::iterator_t<R2>
	>
	concat_view(R1&&, R2&&) -> concat_view<R1, R2>;
}