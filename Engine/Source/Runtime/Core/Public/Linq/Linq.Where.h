// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>

namespace Linq
{
	inline constexpr auto Where = std::views::filter;
}