// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <iterator>

namespace IString_details
{
	template<class T, class TChar>
	concept IStringTyped = requires
	{
		{ std::declval<T>()[std::declval<size_t>()] } -> std::same_as<TChar>;
		{ std::declval<T>().c_str() } -> std::convertible_to<TChar*>;
		{ *std::begin(std::declval<T>()) } -> std::same_as<TChar>;
	};
}

template<class T>
concept IString = (
	IString_details::IStringTyped<T, char> ||
	IString_details::IStringTyped<T, wchar_t> ||
	IString_details::IStringTyped<T, const char> ||
	IString_details::IStringTyped<T, const wchar_t>
	) &&
	requires
{
	{ std::declval<T>().length() } -> std::convertible_to<size_t>;
	{ std::advance(std::declval<T&>(), std::declval<size_t>()) };
};