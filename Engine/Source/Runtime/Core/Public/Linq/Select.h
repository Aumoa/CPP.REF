// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Generic/IEnumerable.h"
#include "Generator.h"

namespace libty::inline Core::Linq
{
	template<class TEnumerable, class TSelector>
	auto Select(const TEnumerable & enumerable, TSelector && selector) requires
		IEnumerable<TEnumerable, EnumerableItem_t<TEnumerable>> &&
		std::invocable<TSelector, EnumerableItem_t<TEnumerable>>
	{
		for (auto& item : enumerable)
		{
			co_yield selector(item);
		}
	}

	template<class TEnumerable, class TSelector>
	auto Select(TEnumerable enumerable, TSelector && selector) requires
		IEnumerable<TEnumerable, EnumerableItem_t<TEnumerable>> &&
		std::invocable<TSelector, EnumerableItem_t<TEnumerable>>
	{
		return Select(std::ref(enumerable), std::forward<TSelector>(selector));
	}
}