// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include <type_traits>
#include <tuple>

namespace libty::inline Core
{
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

	template<size_t N>
	using inh_select = InheritSelector<N>;

	namespace Concepts::Impl
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
	concept SameAsVariadic = Concepts::Impl::SameAsVariadic_t<T, N, TArgs...>::value;

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

	template<class T, class UKey, class UValue>
	concept IDictionary = requires
	{
		{ std::begin(std::declval<T>())->first } -> std::convertible_to<UKey>;
		{ std::begin(std::declval<T>())->second } -> std::convertible_to<UValue>;
		{ std::declval<T>()[std::declval<UKey>()] } -> std::convertible_to<UValue>;
		{ std::begin(std::declval<T>()) != std::end(std::declval<T>()) } -> std::convertible_to<bool>;
	};

	template<class TDictionary>
	using DictionaryKey_t = std::remove_const_t<std::remove_reference_t<decltype(std::begin(std::declval<TDictionary>())->first)>>;

	template<class TDictionary>
	using DictionaryValue_t = std::remove_const_t<std::remove_reference_t<decltype(std::begin(std::declval<TDictionary>())->second)>>;

	namespace Concepts::Impl
	{
		template<class TTuple, class... TCompareTypes>
		struct TupleConvertibleTo
		{
		private:
			template<size_t N, class TCompare, class... TLeft>
			static consteval bool DoCheck() requires requires { std::get<N>(std::declval<TTuple>()); }
			{
				using TCurrent = std::remove_const_t<std::remove_reference_t<decltype(std::get<N>(std::declval<TTuple>()))>>;
				if constexpr (std::same_as<TCurrent, TCompare>)
				{
					if constexpr (sizeof...(TLeft))
					{
						return DoCheck<N + 1, TLeft...>();
					}
					else
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}

			template<size_t N, class TCompare, class... TLeft>
			static consteval bool DoCheck()
			{
				return false;
			}

		public:
			static constexpr bool Value = std::tuple_size<TTuple>::value == sizeof...(TCompareTypes) and DoCheck<0, TCompareTypes...>();
		};
	}

	template<class T, class... TValueTypes>
	concept ITuple = Concepts::Impl::TupleConvertibleTo<T, TValueTypes...>::Value;

	template<class T>
	concept IGenericTuple = requires
	{
		{ std::get<0>(std::declval<T>()) };
		{ std::tuple_size<T>::value } -> std::convertible_to<size_t>;
		{ std::declval<std::tuple_element_t<0, T>>() };
	};

	template<template<class...> class TGenericClass, class... TArguments>
	concept IGenericClass = requires
	{
		{ std::declval<typename TGenericClass<TArguments...>::TGenericVoid>() };
		{ TGenericClass<typename TGenericClass<TArguments...>::TGenericVoid>(std::declval<TGenericClass<TArguments...>>()) };
		{ TGenericClass<TArguments...>(std::declval<TGenericClass<typename TGenericClass<TArguments...>::TGenericVoid>>()) };
	};
}