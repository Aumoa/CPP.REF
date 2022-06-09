// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <tuple>

template<class T>
concept ITuple = requires
{
	{ std::get<0>(std::declval<T>()) };
	{ std::tuple_size_v<T> } -> std::convertible_to<size_t>;
};