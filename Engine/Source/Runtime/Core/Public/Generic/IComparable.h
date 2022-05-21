// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <compare>
#include <concepts>

namespace libty::inline Core
{
	template<class T, class U>
	concept IComparable = requires
	{
		{ std::declval<T>().CompareTo(std::declval<U>()) } -> std::convertible_to<std::strong_ordering>;
	};
}