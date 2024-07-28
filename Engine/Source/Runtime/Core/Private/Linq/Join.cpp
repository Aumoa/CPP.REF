// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Join;

export import :Std;

export namespace Linq
{
	constexpr auto Join = std::views::join;
}
