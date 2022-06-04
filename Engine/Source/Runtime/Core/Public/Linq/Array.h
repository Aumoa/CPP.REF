// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Generic/IArray.h"
#include "Enumerable.h"
#include <array>

template<class TBeginIt, class TEndIt> requires
	requires
	{
		{ std::declval<TEndIt>() - std::declval<TBeginIt>() } -> std::convertible_to<size_t>;
		{ std::declval<TBeginIt>() + std::declval<size_t>() };
		{ std::declval<TBeginIt>() < std::declval<TEndIt>() } -> std::convertible_to<bool>;
	}
class Array : public Enumerable<TBeginIt, TEndIt>
{
	using Super = Enumerable<TBeginIt, TEndIt>;

public:
	constexpr Array(TBeginIt begin, TEndIt end) noexcept
		: Super(begin, end)
	{
	}

	template<class UArray>
	constexpr Array(UArray* enumerable) noexcept requires
		IArray<UArray, EnumerableItem_t<UArray>>
		: Array(enumerable->begin(), enumerable->end())
	{
	}

public:
	template<class U>
	constexpr size_t IndexOf(const U& item) const noexcept requires
		std::equality_comparable_with<typename Super::ValueType, U>
	{
		auto beg = this->begin();
		for (auto it = beg; it != this->end(); ++it)
		{
			if (*it == item)
			{
				return it - beg;
			}
		}
		return -1;
	}

public:
	template<class TIndex>
	constexpr bool IsValidIndex(const TIndex& index) const noexcept requires
		requires
		{
			{ std::declval<Super::IteratorType>() + std::declval<TIndex>() };
		}
	{
		auto indexIt = this->begin() + index;
		return indexIt < this->end();
	}
};

template<class UArray> requires IArray<UArray, EnumerableItem_t<UArray>>
Array(UArray*) -> Array<decltype(std::declval<UArray>().begin()), decltype(std::declval<UArray>().end())>;

namespace Linq
{
	template<class TArray, class... TArgs>
	constexpr auto IndexOf(TArray* enumerable, TArgs&&... args) requires
		requires
	{
		{ Array(enumerable).IndexOf(std::declval<TArgs>()...) };
	}
	{
		return Array(enumerable).IndexOf(std::forward<TArgs>(args)...);
	}
	
	template<class TArray, class... TArgs>
	constexpr auto IsValidIndex(TArray* enumerable, TArgs&&... args) requires
		requires
	{
		{ Array(enumerable).IsValidIndex(std::declval<TArgs>()...) };
	}
	{
		return Array(enumerable).IsValidIndex(std::forward<TArgs>(args)...);
	}
}