// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:SelectMany;

export import :select_many_adaptor;

export namespace Linq::inline views
{
	constexpr adaptors::select_many_adaptor SelectMany;
}