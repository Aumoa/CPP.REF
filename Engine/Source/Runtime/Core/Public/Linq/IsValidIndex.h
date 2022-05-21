// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Generic/IArray.h"

namespace libty::inline Core::Linq
{
	template<class TArray>
	inline bool IsValidIndex(const TArray& list, size_t index) noexcept requires
		IArray<TArray, EnumerableItem_t<TArray>>
	{
		return index < std::size(list);
	}
}