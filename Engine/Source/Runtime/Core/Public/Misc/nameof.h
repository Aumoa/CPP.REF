// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"

namespace lib::details
{
	template<class>
	String __nameof_helper(const String& s)
	{
		return s;
	}
}

#define nameof(x) lib::details::__nameof_helper<decltype(x)>(TEXT(#x))