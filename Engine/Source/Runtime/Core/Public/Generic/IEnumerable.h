// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <algorithm>
#include <concepts>

template<class T, class U>
concept IEnumerable = requires
{
	{ *std::begin(std::declval<T>()) } -> std::convertible_to<U>;
	{ std::begin(std::declval<T>()) != std::end(std::declval<T>()) } -> std::convertible_to<bool>;
	{ std::advance(std::declval<std::remove_reference_t<decltype(std::begin(std::declval<T>()))>&>(), 1) };
};

template<class TEnumerable>
using EnumerableItem_t = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(std::declval<TEnumerable>()))>>;