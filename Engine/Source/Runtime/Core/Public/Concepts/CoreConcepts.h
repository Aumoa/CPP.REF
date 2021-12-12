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