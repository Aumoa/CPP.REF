// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <concepts>

class String;

template<class T>
concept IObject = requires
{
	{ std::declval<std::remove_pointer_t<T>>().ToString() } -> std::convertible_to<String>;
};