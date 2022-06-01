// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IEnumerable.h"

template<class T, class U>
concept IArray = IEnumerable<T, U> && requires (const T& val)
{
	{ val[std::declval<size_t>()] } -> std::convertible_to<U>;
	{ std::size(val) };
	{ std::begin(val) < std::end(val) } -> std::convertible_to<bool>;
	{ std::begin(val) + std::declval<size_t>() };
	{ std::end(val) - std::begin(val) };
};