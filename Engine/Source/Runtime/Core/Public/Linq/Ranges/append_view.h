// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "Linq/Ranges/concat_view.h"
#include "Linq/Ranges/single_view.h"

namespace Ayla::inline Linq::inline Ranges
{
	template<std::ranges::input_range R, class T> requires
		std::constructible_from<concat_view<R, single_view<T>>, R, single_view<T>>
	struct append_view : public std::ranges::view_interface<append_view<R, T>>
	{
	private:
		using view_type = concat_view<R, single_view<T>>;

	public:
		using iterator = typename view_type::iterator;

	private:
		view_type view;

	public:
		template<std::ranges::input_range UR, class UT>
			requires std::constructible_from<
				view_type,
				UR,
				single_view<UT>
			>
		constexpr append_view(UR&& r, UT&& v) noexcept
			: view(std::forward<UR>(r), single_view<UT>(std::forward<UT>(v)))
		{
		}

		constexpr auto begin() noexcept
		{
			return view.begin();
		}

		constexpr auto end() noexcept
		{
			return view.end();
		}

		constexpr auto begin() const noexcept
		{
			return view.begin();
		}

		constexpr auto end() const noexcept
		{
			return view.end();
		}

		constexpr size_t size() const noexcept requires std::ranges::sized_range<R>
		{
			return view.size();
		}
	};

	template<std::ranges::input_range R, class T> requires std::constructible_from<
		concat_view<R, single_view<T>>,
		R,
		single_view<T>
	>
	append_view(R&&, T&&) -> append_view<R, T>;
}