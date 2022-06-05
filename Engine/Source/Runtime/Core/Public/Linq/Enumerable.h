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
	constexpr Enumerable(UEnumerable* enumerable) noexcept requires
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

	constexpr bool Any() const noexcept
	{
		return _begin < _end;
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
	constexpr bool Contains(UPred pred) const noexcept(noexcept(pred(std::declval<ValueType&>()))) requires
		std::invocable<UPred, ValueType&> &&
		std::convertible_to<std::invoke_result_t<UPred, ValueType&>, bool>
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

	constexpr ValueType Min() const requires
		std::three_way_comparable<ValueType>
	{
		auto it = _begin;
		auto minimal = *it++;
		for (; it != _end; ++it)
		{
			auto& value = *it;
			if (minimal > value)
			{
				minimal = value;
			}
		}

		return minimal;
	}

	template<class TCompare>
	constexpr ValueType Min(TCompare compare) const requires
		std::invocable<TCompare, ValueType&, ValueType&> &&
		std::convertible_to<std::invoke_result_t<TCompare, ValueType&, ValueType&>, bool>
	{
		auto it = _begin;
		auto minimal = *it++;
		for (; it != _end; ++it)
		{
			if (compare(minimal, *it))
			{
				minimal = *it;
			}
		}

		return minimal;
	}

private:
	template<class TIterator, class TSelector>
	class SelectIterator
	{
		template<class UIterator, class USelector>
		friend class SelectIterator;

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

		template<class UIterator, class USelector>
		constexpr bool operator <(const SelectIterator<UIterator, USelector>& rhs) const noexcept
		{
			return _it < rhs._it;
		}
	};

public:
	template<class TSelector>
	constexpr auto Select(TSelector selector) const noexcept(noexcept(selector(std::declval<ValueType&>()))) requires
		std::invocable<TSelector, ValueType&> &&
		(!std::same_as<std::invoke_result_t<TSelector, ValueType&>, void>)
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

private:
	template<class TIterator, class TIteratorEnd, class TSelector>
	class WhereIterator
	{
		template<class UIterator, class UIteratorEnd, class USelector>
		friend class WhereIterator;

		TIterator _it;
		TIteratorEnd _end;
		TSelector _selector;

	public:
		constexpr WhereIterator(TIterator it, TIteratorEnd end, TSelector selector) noexcept
			: _it(std::move(it))
			, _end(std::move(end))
			, _selector(std::move(selector))
		{
			while (_it < _end && !_selector(*_it)) {
				++_it;
			}
		}

		constexpr decltype(auto) operator *() const noexcept
		{
			return *_it;
		}

		constexpr WhereIterator& operator ++() noexcept
		{
			while (++_it < _end && !_selector(*_it));
			return *this;
		}

		template<class UIterator, class UEndIt, class USelector>
		constexpr bool operator !=(const WhereIterator<UIterator, UEndIt, USelector>& rhs) const noexcept
		{
			return _it != rhs._it;
		}

		template<class UIterator, class UEndIt, class USelector>
		constexpr bool operator <(const WhereIterator<UIterator, UEndIt, USelector>& rhs) const noexcept
		{
			return _it < rhs._it;
		}
	};

public:
	template<class TSelector>
	constexpr auto Where(TSelector selector) const noexcept(noexcept(selector(std::declval<ValueType&>()))) requires
		std::invocable<TSelector, ValueType&> &&
		std::convertible_to<std::invoke_result_t<TSelector, ValueType&>, bool>
	{
		using UBeginIt = WhereIterator<TBeginIt, TEndIt, TSelector>;
		using UEndIt = WhereIterator<TEndIt, TEndIt, TSelector>;
		return Enumerable<UBeginIt, UEndIt>(UBeginIt(_begin, _end, selector), UEndIt(_end, _end, selector));
	}

};

template<class UEnumerable> requires IEnumerable<UEnumerable, EnumerableItem_t<UEnumerable>>
Enumerable(UEnumerable*) -> Enumerable<decltype(std::declval<UEnumerable>().begin()), decltype(std::declval<UEnumerable>().end())>;

template<class T>
class EnumerableExtensions
{
public:
	template<class TSelf>
	constexpr auto Any(this TSelf& self) requires
		requires
		{
			{ Enumerable(&self).Any() };
		}
	{
		return Enumerable(&self).Any();
	}

	template<class TSelf, class... TArgs>
	constexpr auto Min(this TSelf& self, TArgs&&... args) requires
		requires
		{
			{ Enumerable(&self).Min(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(&self).Min(std::forward<TArgs>(args)...);
	}
	
	template<class TSelf, class... TArgs>
	constexpr auto Contains(this TSelf& self, TArgs&&... args) requires
		requires
		{
			{ Enumerable(&self).Contains(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(&self).Contains(std::forward<TArgs>(args)...);
	}

	template<class TSelf, class... TArgs>
	constexpr auto Select(this TSelf& self, TArgs&&... args) requires
		requires
		{
			{ Enumerable(&self).Select(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(&self).Select(std::forward<TArgs>(args)...);
	}

	template<class TSelf, class U = EnumerableItem_t<TSelf>, class... TArgs>
	constexpr auto ToVector(this TSelf& self, TArgs&&... args) requires
		requires
		{
			{ Enumerable(&self).template ToVector<U>(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(&self).template ToVector<U>(std::forward<TArgs>(args)...);
	}

	template<class TSelf, class... TArgs>
	constexpr auto Where(this TSelf& self, TArgs&&... args) requires
		requires
		{
			{ Enumerable(&self).Where(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(&self).Where(std::forward<TArgs>(args)...);
	}
};

namespace Linq
{
	template<class TEnumerable>
	constexpr auto Any(TEnumerable* enumerable) requires
		requires
		{
			{ Enumerable(enumerable).Any() };
		}
	{
		return Enumerable(enumerable).Any();
	}

	template<class TEnumerable, class... TArgs>
	constexpr auto Min(TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Min(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Min(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class... TArgs>
	constexpr auto Contains(TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Contains(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Contains(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class... TArgs>
	constexpr auto Select(TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Select(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Select(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class U = EnumerableItem_t<TEnumerable>, class... TArgs>
	constexpr auto ToVector(TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).template ToVector<U>(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).template ToVector<U>(std::forward<TArgs>(args)...);
	}

	template<class TEnumerable, class... TArgs>
	constexpr auto Where(TEnumerable* enumerable, TArgs&&... args) requires
		requires
		{
			{ Enumerable(enumerable).Where(std::declval<TArgs>()...) };
		}
	{
		return Enumerable(enumerable).Where(std::forward<TArgs>(args)...);
	}
}