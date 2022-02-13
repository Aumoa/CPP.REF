// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include <type_traits>

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

template<class T>
concept IsGCClass = requires
{
	{ std::remove_reference_t<std::remove_pointer_t<T>>::StaticClass() };
};

template<class TTupleClass>
struct NumGCTypes
{
	static constexpr size_t Value = 0;
};

template<template<class...> class TTupleClass, class TTupleType, class... TTupleTypes>
class NumGCTypes<TTupleClass<TTupleType, TTupleTypes...>>
{
	static consteval size_t GetImpl()
	{
		constexpr bool Test = IsGCClass<TTupleType>;
		if constexpr (sizeof...(TTupleTypes) == 0)
		{
			return Test ? 1 : 0;
		}
		else
		{
			return (Test ? 1 : 0) + NumGCTypes<TTupleClass<TTupleTypes...>>::GetImpl();
		}
	}

public:
	static constexpr size_t Value = GetImpl();
};

template<class T>
concept IsMutableCollection = requires (T & Collection)
{
	{ *Collection.begin() = {} };
};

namespace CoreSObject::Concepts::Impl
{
	template<class T, size_t N, class TForward, class... TArgs>
	struct SameAsVariadic_t : public std::bool_constant<SameAsVariadic_t<T, N - 1, TArgs...>::value>
	{
	};

	template<class T, class TForward, class... TArgs>
	struct SameAsVariadic_t<T, 0, TForward, TArgs...> : public std::bool_constant<std::same_as<T, TForward>>
	{
	};

	template<class T, size_t N, class TForward>
	struct SameAsVariadic_t<T, N, TForward> : public std::bool_constant<false>
	{
	};
}

template<class T, size_t N, class... TArgs>
concept SameAsVariadic = CoreSObject::Concepts::Impl::SameAsVariadic_t<T, N, TArgs...>::value;