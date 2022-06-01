// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"

namespace libty::Syntax
{
	template<class>
	String __nameof_helper(const String& s)
	{
		return s;
	}
}

#define nameof(x) __nameof_helper<decltype(x)>(TEXT(#x))