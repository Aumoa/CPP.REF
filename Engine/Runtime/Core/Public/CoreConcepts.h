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
		TIsSame<TChar, char> ||
		TIsSame<TChar, wchar_t>;

	template<class TPrim>
	concept TIsPrimitive =
		TIsSame<TPrim, uint8> ||
		TIsSame<TPrim, uint16> ||
		TIsSame<TPrim, uint32> ||
		TIsSame<TPrim, uint64> ||
		TIsSame<TPrim, int8> ||
		TIsSame<TPrim, int16> ||
		TIsSame<TPrim, int32> ||
		TIsSame<TPrim, int64> ||
		TIsSame<TPrim, float> ||
		TIsSame<TPrim, double> ||
		TIsSame<TPrim, bool> ||
		TIsSame<TPrim, char> ||
		TIsSame<TPrim, wchar_t>;

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
}