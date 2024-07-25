// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Select;

export import :Std;

export namespace Linq
{
	constexpr auto Select = std::views::transform;
}