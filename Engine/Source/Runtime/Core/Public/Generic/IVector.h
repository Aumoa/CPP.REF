// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IList.h"

namespace libty::inline Core
{
	template<class T, class U>
	concept IVector = IList<T, U> && requires
	{
		{ std::declval<T>()[std::declval<size_t>()] } -> std::convertible_to<U>;
		{ std::declval<T>().reserve(std::declval<size_t>()) };
	};
}