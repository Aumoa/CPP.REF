// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include <type_traits>

namespace CoreSObject::Concepts::Impl
{
	template<class T, size_t N, class TForward, class... TArgs>
	struct SameAsVariadic_t : public std::bool_constant<SameAsVariadic_t<T, N - 1, TArgs...>::value>
	{
		static_assert(N - 1 >= sizeof...(TArgs), "Template arguments not enough.");
	};

	template<class T, class TForward, class... TArgs>
	struct SameAsVariadic_t<T, 0, TForward, TArgs...> : public std::bool_constant<std::same_as<T, TForward>>
	{
	};
}

template<class T, size_t N, class... TArgs>
concept SameAsVariadic = CoreSObject::Concepts::Impl::SameAsVariadic_t<T, N, TArgs...>::value;

template<class T, class TChar>
concept IString = requires
{
	{ std::basic_string_view<TChar>(std::declval<T>()) };
};

template<class TString>
using StringChar_t = std::remove_const_t<std::remove_reference_t<decltype(std::declval<TString>()[0])>>;

template<class TUnaryOp, class TChar>
concept ICharTransform = requires
{
	{ std::declval<TUnaryOp>()(std::declval<TChar>()) } -> std::convertible_to<TChar>;
};

template<class T, class U>
concept IEnumerable = requires
{
	{ *std::begin(std::declval<T>()) } -> std::convertible_to<U>;
	{ std::begin(std::declval<T>()) != std::end(std::declval<T>()) } -> std::convertible_to<bool>;
	{ std::advance(std::declval<std::remove_reference_t<decltype(std::begin(std::declval<T>()))>&>(), 1) };
};

template<class T, class U>
concept IList = IEnumerable<T, U> && requires
{
	{ std::size(std::declval<T>()) } -> std::convertible_to<size_t>;
	{ std::declval<T>().emplace_back(std::declval<U>()) };
};

template<class T, class U>
concept IVector = IList<T, U> && requires
{
	{ std::declval<T>()[std::declval<size_t>()] } -> std::convertible_to<U>;
	{ std::declval<T>().reserve(std::declval<size_t>()) };
};

template<class TEnumerable>
using EnumerableItem_t = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(std::declval<TEnumerable>()))>>;

template<class TBody, class... TArgs>
using FunctionReturn_t = decltype(std::declval<TBody>()(std::declval<TArgs>()...));

template<class TBody, class... TArgs>
concept IInvoke = requires
{
	{ std::declval<TBody>()(std::declval<TArgs>()...) };
};