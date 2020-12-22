// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include <type_traits>

namespace SC::Runtime::Core
{
	class Object;
	class String;

	template<class TDerived, class TBase>
	concept TIsBaseOf = requires(TBase* Base, TDerived* Derived)
	{
		{ Base = Derived };
	};

	template<class TAssign, class TBase>
	concept TIsAssignable = requires(const TBase* Base, const TAssign* Assign)
	{
		{ Base = Assign };
	};

	template<class T>
	concept TIsRefCore = TIsBaseOf<T, Object> || TIsBaseOf<T, String>;

	template<class TLeft, class TRight>
	concept TIsSame = std::is_same_v<TLeft, TRight>;

	template<class TChar>
	concept TIsChar =
		TIsSame<std::remove_const_t<TChar>, char> ||
		TIsSame<std::remove_const_t<TChar>, wchar_t>;

	template<class TPrim>
	concept TIsPrimitive =
		TIsSame<std::remove_const_t<TPrim>, uint8> ||
		TIsSame<std::remove_const_t<TPrim>, uint16> ||
		TIsSame<std::remove_const_t<TPrim>, uint32> ||
		TIsSame<std::remove_const_t<TPrim>, uint64> ||
		TIsSame<std::remove_const_t<TPrim>, int8> ||
		TIsSame<std::remove_const_t<TPrim>, int16> ||
		TIsSame<std::remove_const_t<TPrim>, int32> ||
		TIsSame<std::remove_const_t<TPrim>, int64> ||
		TIsSame<std::remove_const_t<TPrim>, float> ||
		TIsSame<std::remove_const_t<TPrim>, double> ||
		TIsSame<std::remove_const_t<TPrim>, bool> ||
		TIsSame<std::remove_const_t<TPrim>, char> ||
		TIsSame<std::remove_const_t<TPrim>, wchar_t>;

	template<class TCallable>
	concept TIsCallable = requires(TCallable Callable)
	{
		{ Callable() };
	};

	template<class TIndexable, class TArg>
	concept TIsIndexer = requires(TIndexable Indexable, TArg Index)
	{
		{ Indexable[Index] };
	};

	template<class T>
	concept TIsPointer = std::is_pointer_v<T>;

	template<class T>
	concept TIsNotPointer = !std::is_pointer_v<T>;

	template<class T, class... TArgs>
	concept THasConstructor = requires(TArgs... args)
	{
		{ T(args...) };
	};

	template<class TLeft, class TRight>
	concept TIsSameWithoutDeco = TIsSame<
		std::remove_const_t<std::remove_reference_t<TLeft>>,
		std::remove_const_t<std::remove_reference_t<TRight>>
	>;

	template<class TIterator, class T>
	concept TIsIterator =
		TIsSameWithoutDeco<decltype(*TIterator()), T> &&
		requires(TIterator& It)
		{
			{ (T)*(++It) };
		} &&
		requires(TIterator& It)
		{
			{ (T)*(It++) };
		} &&
		requires(TIterator& It)
		{
			{ (size_t)(It - It) };
		};

	template<class TStringArg>
	concept TIsStringConstructible = requires(const std::remove_reference_t<TStringArg>& UnaryArg)
	{
		{ String(UnaryArg) };
	};

	template<class T>
	concept THasConstIterator = requires()
	{
		{ typename T::ConstIterator() };
	};

	template<class T>
	concept THasIterator = THasConstIterator<T> && requires()
	{
		{ typename T::Iterator() };
	};

	template<class T>
	concept THasStdEnumerable = requires(const T Container)
	{
		{ std::begin(Container) };
		{ std::begin(Container) };
	};

	template<class T, class O>
	concept TIsConvertible = requires(T Arg)
	{
		{ (O)Arg };
	};

	template<class T>
	concept TIsRealType = TIsConvertible<T, float> || TIsConvertible<T, double> || TIsConvertible<T, long double>;
}