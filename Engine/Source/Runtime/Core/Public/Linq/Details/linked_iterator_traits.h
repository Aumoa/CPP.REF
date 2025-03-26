// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>
#include "IntegralTypes.h"

#define VALUE_TYPE_TRAITS(T) typename std::iterator_traits<T>::value_type

namespace Ayla::inline Linq::details
{
	template<std::input_iterator T1, std::input_iterator T2>
	struct linked_iterator_traits
	{
	};

	template<std::input_iterator T1, std::input_iterator T2>
		requires std::convertible_to<
			VALUE_TYPE_TRAITS(T2),
			VALUE_TYPE_TRAITS(T1)
		> && (!std::convertible_to<
			VALUE_TYPE_TRAITS(T1),
			VALUE_TYPE_TRAITS(T2)
		>)
	struct linked_iterator_traits<T1, T2> : public std::iterator_traits<T1>
	{
		using base = std::iterator_traits<T1>;
		using iterator_category = std::forward_iterator_tag;
		using value_type = const std::remove_cv_t<typename base::value_type>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		static constexpr bool is_same = false;
	};

	template<std::input_iterator T1, std::input_iterator T2>
		requires std::convertible_to<
			VALUE_TYPE_TRAITS(T1),
			VALUE_TYPE_TRAITS(T2)
		> && (!std::convertible_to<
			VALUE_TYPE_TRAITS(T2),
			VALUE_TYPE_TRAITS(T1)
		>)
	struct linked_iterator_traits<T1, T2> : public std::iterator_traits<T2>
	{
		using base = std::iterator_traits<T2>;
		using iterator_category = std::forward_iterator_tag;
		using value_type = const std::remove_cv_t<typename base::value_type>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		static constexpr bool is_same = false;
	};


	template<std::input_iterator T1, std::input_iterator T2>
		requires std::convertible_to<
			VALUE_TYPE_TRAITS(T2),
			VALUE_TYPE_TRAITS(T1)
		> && (
			sizeof(VALUE_TYPE_TRAITS(T1)) >
			sizeof(VALUE_TYPE_TRAITS(T2))
		)
	struct linked_iterator_traits<T1, T2> : public std::iterator_traits<T1>
	{
		using base = std::iterator_traits<T1>;
		using iterator_category = std::forward_iterator_tag;
		using value_type = const std::remove_cv_t<typename base::value_type>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		static constexpr bool is_same = false;
	};

	template<std::input_iterator T1, std::input_iterator T2>
		requires std::convertible_to<
			VALUE_TYPE_TRAITS(T1),
			VALUE_TYPE_TRAITS(T2)
		> && (
			sizeof(VALUE_TYPE_TRAITS(T2)) >
			sizeof(VALUE_TYPE_TRAITS(T1))
		)
	struct linked_iterator_traits<T1, T2> : public std::iterator_traits<T2>
	{
		using base = std::iterator_traits<T2>;
		using iterator_category = std::forward_iterator_tag;
		using value_type = const std::remove_cv_t<typename base::value_type>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		static constexpr bool is_same = false;
	};

	template<std::input_iterator T1, std::input_iterator T2>
		requires std::same_as<VALUE_TYPE_TRAITS(T1), VALUE_TYPE_TRAITS(T2)>
	struct linked_iterator_traits<T1, T2> : public std::iterator_traits<T1>
	{
		using base = std::iterator_traits<T1>;
		using iterator_category = std::forward_iterator_tag;
		using value_type = const std::remove_cv_t<typename base::value_type>;
		using difference_type = std::ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		static constexpr bool is_same = true;
	};
}

#undef VALUE_TYPE_TRAITS