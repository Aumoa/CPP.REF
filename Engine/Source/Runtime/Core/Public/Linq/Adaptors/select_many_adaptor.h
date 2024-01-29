// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Linq/Ranges/select_many_view.h"

namespace Linq::adaptors
{
	template<class T>
	struct select_many_adaptor_closure
	{
		T value;

		template<class U>
		constexpr select_many_adaptor_closure(U&& value) noexcept
			: value(std::forward<U>(value))
		{
		}

		template<
			std::ranges::input_range R,
			class R_iterator = std::ranges::iterator_t<R>,
			class R_value = typename std::iterator_traits<R_iterator>::value_type,
			class R_view = std::invoke_result_t<T, R_value>
		>
		constexpr auto operator ()(R&& view) const noexcept
		{
			return ranges::select_many_view(std::forward<R>(view), value);
		}
	};

	template<class T>
	select_many_adaptor_closure(T&&) -> select_many_adaptor_closure<T>;

	struct select_many_adaptor
	{
		template<class T>
		constexpr auto operator ()(T&& value) const noexcept
		{
			return select_many_adaptor_closure(std::forward<T>(value));
		}
	};

	template<std::ranges::input_range R, class T> requires requires { std::declval<select_many_adaptor_closure<T>&>(); }
	constexpr auto operator |(R&& view, select_many_adaptor_closure<T>&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}
}