// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:SupportsObject;

import std.core;

/// <summary>
/// Supports native object that provide object's functions.
/// </summary>
export
template<class T>
concept SupportsObject = requires(T instance)
{
	{ instance.ToString(std::declval<std::wstring_view>()) } -> std::same_as<std::wstring>;
};