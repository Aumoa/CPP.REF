// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>

namespace Linq::adaptors
{
	template<class T>
	struct contains_adaptor_closure_element_or_predicate
	{
		T value;

		template<class U>
		constexpr contains_adaptor_closure_element_or_predicate(U&& value) noexcept
			: value(std::forward<U>(value))
		{
		}

		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& view) const noexcept requires std::invocable<T, typename std::iterator_traits<std::ranges::iterator_t<R>>::value_type>
		{
			for (auto& v : view)
			{
				if (value(v))
				{
					return true;
				}
			}

			return false;
		}

		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& view) const noexcept requires
			requires
		{
			{ std::declval<T>() == std::declval<typename std::iterator_traits<std::ranges::iterator_t<R>>::value_type>() } -> std::convertible_to<bool>;
		}
		{
			for (auto& v : view)
			{
				if (value == v)
				{
					return true;
				}
			}

			return false;
		}
	};

	template<class T>
	contains_adaptor_closure_element_or_predicate(T&&) -> contains_adaptor_closure_element_or_predicate<T>;

	struct contains_adaptor
	{
		template<class T>
		constexpr auto operator ()(T&& value) const noexcept
		{
			return contains_adaptor_closure_element_or_predicate(std::forward<T>(value));
		}
	};

	template<std::ranges::input_range R, class T> requires
		requires
	{
		{ std::declval<T>() == std::declval<typename std::iterator_traits<std::ranges::iterator_t<R>>::value_type>() } -> std::convertible_to<bool>;
	}
	constexpr auto operator |(R&& view, contains_adaptor_closure_element_or_predicate<T>&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}

	template<std::ranges::input_range R, std::invocable<typename std::iterator_traits<std::ranges::iterator_t<R>>::value_type> T>
	constexpr auto operator |(R&& view, contains_adaptor_closure_element_or_predicate<T>&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}
}