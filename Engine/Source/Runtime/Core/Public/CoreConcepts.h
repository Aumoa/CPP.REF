﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include <type_traits>
#include <string>
#include <string_view>

class Object;
class String;

template<class TDerived, class TBase>
concept TIsBaseOf = requires(TBase* Base, TDerived* Derived)
{
	{ Base = Derived };
};

template<class TAssign, class TBase>
concept TIsAssignable = requires(TBase Base, TAssign Assign)
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
	TIsSame<std::remove_const_t<TPrim>, float> ||
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
concept TIsStringConstructible =
	TIsAssignable<TStringArg, const char*> ||
	TIsAssignable<TStringArg, const wchar_t*> ||
	TIsAssignable<TStringArg, const std::string&> ||
	TIsAssignable<TStringArg, const std::wstring&> ||
	TIsAssignable<TStringArg, std::string_view> ||
	TIsAssignable<TStringArg, std::wstring_view>;

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
concept TIsVectorType = requires(T Arg, const T ConstArg)
{
	{ ConstArg.GetComponentOrDefault(0) };
	{ (bool)ConstArg.Contains(0) };
	{ (float)ConstArg[0] };
	{ (size_t)ConstArg.Count };
};

template<class T, class O>
concept TIsSmallThen = sizeof(T) < sizeof(O);

template<class T, class O>
concept TIsGreaterEqualsThen = sizeof(T) >= sizeof(O);

template<class T, class O>
concept TComparable_Less = requires(T lh, O rh)
{
	{ lh < rh };
};

template<class T, class O>
concept TComparable_LessEquals = requires(T lh, O rh)
{
	{ lh <= rh };
};

template<class T, class O>
concept TComparable_Greater = requires(T lh, O rh)
{
	{ lh > rh };
};

template<class T, class O>
concept TComparable_GreaterEquals = requires(T lh, O rh)
{
	{ lh >= rh };
};

#pragma warning(push)
#pragma warning(disable: 4305)
template<class T>
constexpr bool IsRealType = ((T)(1.0 + 0.1)) != ((T)1.0);
#pragma warning(pop)

template<class T>
concept TIsRealType = IsRealType<T>;

template<class T>
concept TIsMatrixType = requires(T Arg, const T ConstArg)
{
	{ ConstArg.GetComponentOrDefault(0) };
	{ (bool)ConstArg.Contains(0) };
	{ (bool)ConstArg.Contains(0, 0) };
	{ (float)ConstArg[0] };
	{ (float)ConstArg[0][0] };
	{ (size_t)ConstArg.Rows };
	{ (size_t)ConstArg.Columns };
	{ (size_t)ConstArg.Count };
};

template<class T>
concept TIsObject = requires(Object* Base, T* Derived)
{
	{ Base = Derived };
};