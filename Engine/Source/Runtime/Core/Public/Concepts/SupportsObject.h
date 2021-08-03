// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include <type_traits>

/// <summary>
/// Supports native object that provide object's functions.
/// </summary>
template<class T>
concept SupportsObject = requires(T instance)
{
	{ instance.ToString(std::declval<std::wstring_view>()) } -> std::same_as<std::wstring>;
};