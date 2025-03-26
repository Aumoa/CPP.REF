// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Linq/Ranges/select_many_view.h"

namespace Ayla::inline Linq::Adaptors
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
			return Ranges::select_many_view(std::forward<R>(view), value);
		}

		template<std::ranges::input_range R, class T> requires requires { std::declval<Ayla::Linq::Adaptors::select_many_adaptor_closure<T>&>(); }
		friend constexpr auto operator |(R&& view, Ayla::Linq::Adaptors::select_many_adaptor_closure<T>&& adaptor) noexcept
		{
			return adaptor(std::forward<R>(view));
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
}