// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Where;

export import :Std;

export namespace Linq
{
	inline constexpr auto Where = std::views::filter;
}