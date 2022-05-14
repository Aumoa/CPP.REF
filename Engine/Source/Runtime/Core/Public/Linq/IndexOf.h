// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Concepts/IArray.h"

namespace libty::inline Core::Linq
{
	template<class TArray, class TObject>
	constexpr size_t IndexOf(const TArray & array, const TObject & item) requires
		IArray<TArray, std::remove_reference_t<TObject>> &&
		requires { { std::declval<const TObject&>() == std::declval<const TObject&>() } -> std::convertible_to<bool>; }
	{
		size_t size = std::size(array);

		for (size_t i = 0; i < size; ++i)
		{
			if (array[i] == item)
			{
				return i;
			}
		}

		return -1;
	}

	template<class TArray, class TPred>
	constexpr bool IndexOf(const TArray & array, TPred && pred) requires
		IArray<TArray, EnumerableItem_t<TArray>> &&
		std::invocable<TPred, EnumerableItem_t<TArray>> &&
		std::convertible_to<std::invoke_result_t<TPred, EnumerableItem_t<TArray>>, bool>
	{
		size_t size = std::size(array);

		for (size_t i = 0; i < size; ++i)
		{
			if (pred(array[i]))
			{
				return i;
			}
		}

		return -1;
	}
}