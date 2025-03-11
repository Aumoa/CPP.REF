// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <ranges>

namespace Linq
{
	inline constexpr auto Where = std::views::filter;
}