// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <utility>

namespace libty::inline Core::inline Casts
{
	namespace Impl
	{
		template<class TTo, class TFrom>
		inline static TTo DoCast(short, TFrom&& from) requires requires
		{
			{ static_cast<TTo>(std::declval<TFrom>()) };
		}
		{
			return static_cast<TTo>(std::forward<TFrom>(from));
		}

		template<class TTo, class TFrom>
		static TTo DoCast(int, TFrom&& from) requires requires
		{
			{ String::Cast<TTo>(std::declval<TFrom>()) };
		}
		{
			return String::Cast<TTo>(std::forward<TFrom>(from));
		}
	}

	template<class TTo, class TFrom>
	inline auto Cast(TFrom&& from) requires requires
	{
		{ libty::Core::Casts::Impl::DoCast<TTo, TFrom>(std::declval<int>(), std::declval<TFrom>()) };
	}
	{
		return libty::Core::Casts::Impl::DoCast<TTo, TFrom>(0, std::forward<TFrom>(from));
	}
}