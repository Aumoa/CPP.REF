// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <concepts>

namespace libty::inline Core
{
	template<class T, class U>
	concept IEquatable = requires
	{
		{ std::declval<T>().Equals(std::declval<U>()) } -> std::convertible_to<bool>;
	};
}