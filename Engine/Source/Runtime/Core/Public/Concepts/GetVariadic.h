// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Variadic.h"
#include <concepts>

template<size_t Index, class T, class... TArgs>
struct GetVariadic
{
	using Type = typename GetVariadic<Index - 1, TArgs...>::Type;
};

template<size_t Index, class... TArgs>
struct GetVariadic<Index, Variadic<TArgs...>> : public GetVariadic<Index, TArgs...>
{
};

template<class T, class... TArgs>
struct GetVariadic<0, T, TArgs...>
{
	using Type = T;
};

template<class... TArgs>
struct GetVariadic<0, Variadic<TArgs...>> : public GetVariadic<0, TArgs...>
{
};

template<size_t Index, class... TArgs>
using GetVariadic_t = typename GetVariadic<Index, TArgs...>::Type;