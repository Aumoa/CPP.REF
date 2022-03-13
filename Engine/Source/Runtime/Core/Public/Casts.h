// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <utility>

namespace libty::Casts::Impl
{
	template<class TTo, class TFrom> requires requires
	{
		{ static_cast<TTo>(std::declval<TFrom>()) };
	}
	inline static TTo DoCast(TFrom&& from)
	{
		return static_cast<TTo>(std::forward<TFrom>(from));
	}

	template<class TTo, class TFrom> requires requires
	{
		{ String::Cast<TTo>(std::declval<TFrom>()) };
	}
	static TTo DoCast(TFrom&& from)
	{
		return String::Cast<TTo>(std::forward<TFrom>(from));
	}
}

template<class TTo, class TFrom>
inline auto Cast(TFrom&& from)
{
	return libty::Casts::Impl::DoCast<TTo, TFrom>(0, std::forward<TFrom>(from));
}