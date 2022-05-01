// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"

namespace libty::inline Core::Linq
{
	template<class TEnumerable, class TObject>
	bool Contains(const TEnumerable& enumerable, const TObject& item) requires
		IEnumerable<TEnumerable, std::remove_reference_t<TObject>> &&
		requires { { std::declval<const TObject&>() == std::declval<const TObject&>() } -> std::convertible_to<bool>; }
	{
		for (auto& at : enumerable)
		{
			if (at == item)
			{
				return true;
			}
		}

		return false;
	}

	template<class TEnumerable, class TPred>
	bool Contains(const TEnumerable& enumerable, TPred&& pred) requires
		IEnumerable<TEnumerable, EnumerableItem_t<TEnumerable>> &&
		std::invocable<TPred, EnumerableItem_t<TEnumerable>> &&
		std::convertible_to<std::invoke_result_t<TPred, EnumerableItem_t<TEnumerable>>, bool>
	{
		for (auto& at : enumerable)
		{
			if (pred(at))
			{
				return true;
			}
		}

		return false;
	}
}