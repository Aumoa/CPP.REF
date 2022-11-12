// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

template<class... T>
struct Variadic
{
	static constexpr const size_t Count = sizeof...(T);
};