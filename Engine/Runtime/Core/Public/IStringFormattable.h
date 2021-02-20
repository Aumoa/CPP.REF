// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

template<class T>
struct TIsFormattableStringConvertibleType
{
	template<class O> requires requires(const O& arg)
	{
		{ arg.ToString(TRefPtr<String>()) };
	}
	constexpr bool IsImpl(const T& arg)
	{
		arg.ToString(TRefPtr<String>());
		return true;
	}
};

template<class T>
concept TIsFormattableStringConvertible = requires(const T& arg)
{
	{ arg.ToString() };
	{ arg.ToString(TRefPtr<String>()) };
};

interface IStringFormattable : virtual public Object
{
	using Object::ToString;

	virtual TRefPtr<String> ToString(TRefPtr<String> formatText) const = 0;
};