// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>

template<class TDerived, class TBase>
concept IDerivedFrom = requires
{
	{ std::declval<TBase*&>() = std::declval<TDerived*&>() };
};