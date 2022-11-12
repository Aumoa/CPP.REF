// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

template<class TBase, class... TArgs>
concept CompatibleVariadic = requires (TArgs&&... vals)
{
	{ std::initializer_list<TBase>{ TBase(vals)... } };
};