// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <ranges>

namespace Linq
{
	inline namespace views
	{
		constexpr auto Values = std::views::values;
	}
}