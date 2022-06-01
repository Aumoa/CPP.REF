// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

template<size_t Index, class T, class... TArgs>
struct GetVariadic
{
	using Type = typename GetVariadic<Index - 1, TArgs...>::Type;
};

template<class T, class... TArgs>
struct GetVariadic<0, T, TArgs...>
{
	using Type = T;
};

template<size_t Index, class... TArgs>
using GetVariadic_t = typename GetVariadic<Index, TArgs...>::Type;