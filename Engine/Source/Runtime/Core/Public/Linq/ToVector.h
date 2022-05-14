// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Concepts/IEnumerable.h"
#include <vector>

namespace libty::inline Core::Linq
{
	template<class TEnumerable>
	auto ToVector(const TEnumerable & enumerable) requires
		IEnumerable<TEnumerable, EnumerableItem_t<TEnumerable>>
	{
		using T = EnumerableItem_t<TEnumerable>;

		std::vector<T> v;
		for (const T& item : enumerable)
		{
			v.emplace_back(item);
		}
		return v;
	}
}