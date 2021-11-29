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

template<class T, class... TArgs>
concept Constructible = requires(TArgs&&... args)
{
	{ T(std::forward<TArgs>(args)...) };
};

template<class T>
concept HasSuper = requires { typename T::Super; } && !std::same_as<typename T::Super, void>;

template<size_t _Depth>
struct InheritSelector : public InheritSelector<_Depth - 1>
{
	constexpr static size_t Depth = _Depth;
};

template<>
struct InheritSelector<0>
{
	constexpr static size_t Depth = 0;
};