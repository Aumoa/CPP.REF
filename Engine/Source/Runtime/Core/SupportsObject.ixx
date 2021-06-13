// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:SupportsObject;

import std.core;

using namespace std;

/// <summary>
/// Supports native object that provide object's functions.
/// </summary>
export
template<class T>
concept SupportsObject = requires(T instance)
{
	{ instance.ToString(declval<wstring_view>()) } -> same_as<wstring>;
};