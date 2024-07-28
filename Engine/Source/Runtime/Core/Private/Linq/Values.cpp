// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Values;

export import :Std;

export namespace Linq::inline views
{
	constexpr auto Values = std::views::values;
}