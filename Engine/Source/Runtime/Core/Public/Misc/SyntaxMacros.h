// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"

template<class>
::libty::String __nameof_helper(::libty::String s)
{
	return s;
}

#define nameof(x) __nameof_helper<decltype(x)>(TEXT(#x))