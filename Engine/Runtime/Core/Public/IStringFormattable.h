// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

namespace SC::Runtime::Core
{
	template<class T>
	concept TIsFormattableStringConvertible = requires(const T& arg)
	{
		{ arg.ToString(TRefPtr<String>()) };
	};

	interface IStringFormattable : virtual public Object
	{
		using Object::ToString;

		virtual TRefPtr<String> ToString(TRefPtr<String> formatText) const = 0;
	};
}