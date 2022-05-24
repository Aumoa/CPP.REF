// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IEnumerable.h"

template<class T, class U>
concept IArray = IEnumerable<T, U> && requires
{
	{ std::declval<T>()[std::declval<size_t>()] } -> std::convertible_to<U>;
	{ std::size(std::declval<T>()) } -> std::convertible_to<size_t>;
	{ std::declval<T>().begin() < std::declval<T>().end() } -> std::convertible_to<bool>;
	{ std::declval<T>().begin() + std::declval<size_t>() };
	{ std::declval<T>().end() - std::declval<T>().begin() } -> std::convertible_to<size_t>;
};