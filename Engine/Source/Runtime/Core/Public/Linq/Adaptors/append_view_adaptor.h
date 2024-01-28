// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "Linq/Ranges/concat_view.h"

namespace Linq::adaptors
{
	template<class T>
	struct append_adaptor_closure
	{
		T value;

		constexpr append_adaptor_closure(T&& right_value) noexcept
			: right_value(std::forward<T>(right_value))
		{
		}

		template<std::ranges::input_range R, class V = std::ranges::single_view<T>> requires
			requires
			{
				typename details::linked_iterator_traits<typename R::iterator, typename V::iterator>::value_type;
			}
		constexpr auto operator ()(R&& left_view) const noexcept
		{
			return concat_view(std::forward<R>(left_view), std::views::single(right_value));
		}
	};

	template<class T>
	append_adaptor_closure(T&&) -> append_adaptor_closure<T>;
}