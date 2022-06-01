// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <algorithm>
#include <concepts>

template<class T, class U>
concept IEnumerable = requires (const T& val)
{
	{ *std::begin(val) } -> std::convertible_to<U>;
	{ std::begin(val) != std::end(val) } -> std::convertible_to<bool>;
	{ std::advance(std::declval<std::remove_reference_t<decltype(std::begin(val))>&>(), 1) };
};

template<class TEnumerable>
using EnumerableItem_t = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(std::declval<TEnumerable>()))>>;