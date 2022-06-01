// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

template<class... TArgs>
struct ContainsRef_s
{
	static constexpr bool Value = false;
};

template<class T>
struct ContainsRef_s<T>
{
	static constexpr bool Value = std::is_reference_v<T>;
};

template<class T, class... TArgs>
struct ContainsRef_s<T, TArgs...>
{
	static constexpr bool Value = ContainsRef_s<T>::Value || ContainsRef_s<TArgs...>::Value;
};

template<class... TArgs>
concept ContainsRef = ContainsRef_s<TArgs...>::Value;