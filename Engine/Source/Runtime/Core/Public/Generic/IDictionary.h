// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IEnumerable.h"
#include <map>

template<class T, class TKey, class TValue>
concept IDictionary = IEnumerable<T, std::pair<TKey, TValue>> && requires
{
	{ std::declval<T>()[std::declval<TKey>()] } -> std::convertible_to<TValue>;
	{ std::declval<T>().find(std::declval<TKey>()) } -> std::convertible_to<decltype(std::declval<T>().end())>;
};

template<class T>
using DictionaryKey_t = typename T::key_type;
template<class T>
using DictionaryValue_t = typename T::value_type;