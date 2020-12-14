// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

namespace SC::Runtime::Core
{
	class Object;
	class String;

	namespace Impl
	{
		template<class TLeft, class TRight>
		struct TIsSame
		{
			constexpr static const bool Value = false;
		};

		template<class T>
		struct TIsSame<T, T>
		{
			constexpr static const bool Value = true;
		};
	}

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
	concept TIsSame = Impl::TIsSame<TLeft, TRight>::Value;

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
}