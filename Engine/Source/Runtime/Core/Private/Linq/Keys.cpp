// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:Keys;

export import :Std;

export namespace Linq::inline views
{
	constexpr auto Keys = std::views::keys;
}