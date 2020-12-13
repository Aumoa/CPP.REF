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
	concept TIsBaseOf = requires(const TBase* Base, const TDerived* Derived)
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
}