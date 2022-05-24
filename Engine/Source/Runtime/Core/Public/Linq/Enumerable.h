// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Generic/IEnumerable.h"
#include "Linq/Generator.h"
#include <vector>
#include <span>

template<class TBeginIt, class TEndIt>
class Enumerable
{
	TBeginIt _begin;
	TEndIt _end;

public:
	using ValueType = std::remove_reference_t<decltype(*std::declval<TBeginIt>())>;
	using IteratorType = TBeginIt;

public:
	constexpr Enumerable(TBeginIt begin, TEndIt end) noexcept
		: _begin(std::move(begin))
		, _end(std::move(end))
	{
	}

	template<class UEnumerable>
	constexpr Enumerable(const UEnumerable* enumerable) noexcept requires
		IEnumerable<UEnumerable, EnumerableItem_t<UEnumerable>>
		: Enumerable(enumerable->begin(), enumerable->end())
	{
	}

	constexpr decltype(auto) begin() const noexcept
	{
		return _begin;
	}

	constexpr decltype(auto) end() const noexcept
	{
		return _end;
	}

	template<class U>
	constexpr bool Contains(const U& item) const noexcept requires
		std::equality_comparable_with<ValueType, U>
	{
		for (auto it = _begin; it != _end; ++it)
		{
			if (*it == item)
			{
				return true;
			}
		}

		return false;
	}

	template<class UPred>
	constexpr bool Contains(UPred pred) const noexcept(noexcept(pred(std::declval<ValueType>()))) requires
		std::invocable<UPred, ValueType> &&
		std::convertible_to<std::invoke_result_t<UPred, ValueType>, bool>
	{
		for (auto it = _begin; it != _end; ++it)
		{
			if (pred(*it))
			{
				return true;
			}
		}

		return false;
	}

private:
	template<class TIterator, class TSelector>
	class SelectIterator
	{
		TIterator _it;
		TSelector _selector;

	public:
		constexpr SelectIterator(TIterator it, TSelector selector) noexcept
			: _it(std::move(it))
			, _selector(std::move(selector))
		{
		}

		constexpr decltype(auto) operator *() const noexcept
		{
			return _selector(*_it);
		}

		constexpr SelectIterator& operator ++() noexcept
		{
			++_it;
			return *this;
		}

		template<class UIterator, class USelector>
		constexpr bool operator !=(const SelectIterator<UIterator, USelector>& rhs) const noexcept
		{
			return _it != rhs._it;
		}
	};

public:
	template<class TSelector>
	constexpr auto Select(TSelector selector) const noexcept(noexcept(selector(std::declval<ValueType>()))) requires
		std::invocable<TSelector, ValueType> &&
		(!std::same_as<std::invoke_result_t<TSelector, ValueType>, void>)
	{
		using UBeginIt = SelectIterator<TBeginIt, TSelector>;
		using UEndIt = SelectIterator<TEndIt, TSelector>;
		return Enumerable<UBeginIt, UEndIt>(UBeginIt(_begin, selector), UEndIt(_end, selector));
	}

private:
	template<class UBeginIt, class UEndIt>
	constexpr static size_t SizeOfScope(const UBeginIt& begin, const UEndIt& end) requires
		requires
		{
			{ std::declval<UEndIt>() - std::declval<UBeginIt>() } -> std::convertible_to<size_t>;
		}
	{
		return (size_t)(end - begin);
	}

	template<class UBeginIt, class UEndIt>
	constexpr static size_t SizeOfScope(const UBeginIt& begin, const UEndIt& end)
	{
		return 0;
	}

public:
	template<class U = ValueType>
	constexpr std::vector<U> ToVector() const requires
		std::constructible_from<U, ValueType>
	{
		std::vector<ValueType> values;
		values.reserve(SizeOfScope(_begin, _end));
		for (auto it = _begin; it != _end; ++it)
		{
			values.emplace_back(*it);
		}
		return values;
	}
};

template<class UEnumerable> requires IEnumerable<UEnumerable, EnumerableItem_t<UEnumerable>>
Enumerable(const UEnumerable*) -> Enumerable<decltype(std::declval<const UEnumerable>().begin()), decltype(std::declval<const UEnumerable>().end())>;

namespace Linq
{
	template<class TEnumerable, class... TArgs>
	constexpr auto Contains(const TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Contains(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Contains(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class... TArgs>
	constexpr auto Select(const TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Select(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Select(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class U = EnumerableItem_t<TEnumerable>, class... TArgs>
	constexpr auto ToVector(const TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).ToVector<U>(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).ToVector<U>(std::forward<TArgs>(args)...);
	}
}