// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:Array;

export import :IArray;
export import :Enumerable;
export import <array>;

export template<class TBeginIt = void, class TEndIt = void> requires (
	requires
	{
		{ std::declval<TEndIt>() - std::declval<TBeginIt>() } -> std::convertible_to<size_t>;
		{ std::declval<TBeginIt>() + std::declval<size_t>() };
		{ std::declval<TBeginIt>() < std::declval<TEndIt>() } -> std::convertible_to<bool>;
	}) || (
		std::same_as<TBeginIt, void> &&
		std::same_as<TEndIt, void>
	)
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
			{ std::declval<typename Super::IteratorType>() + std::declval<TIndex>() };
		}
	{
		auto indexIt = this->begin() + index;
		return indexIt < this->end();
	}
};

template<class UArray> requires IArray<UArray, EnumerableItem_t<UArray>>
Array(UArray*) -> Array<decltype(std::declval<UArray>().begin()), decltype(std::declval<UArray>().end())>;

template<>
class Array<void, void>
{
private:
	template<class T>
	static constexpr ptrdiff_t length(const T& val) requires requires { { val.length() }; }
	{
		return (ptrdiff_t)val.length();
	}

	template<class T>
	static constexpr ptrdiff_t length(const T& val)
	{
		return std::ssize(val);
	}

public:
	template<class TArray, class TIntegral>
	static constexpr auto At(const TArray& arr, TIntegral index) -> decltype(auto) requires
		std::integral<TIntegral>
	{
		size_t off = (size_t)index;

		if constexpr (std::signed_integral<TIntegral>)
		{
			ptrdiff_t len = length(arr);
			off = len + (ptrdiff_t)index;
		}

		return arr[off];
	}
};

//template<class T>
//class ArrayExtensions
//{
//public:
//	template<class TSelf, class... TArgs>
//	constexpr auto IndexOf(this TSelf& self, TArgs&&... args) requires
//		requires
//	{
//		{ Array(&self).IndexOf(std::declval<TArgs>()...) };
//	}
//	{
//		return Array(&self).IndexOf(std::forward<TArgs>(args)...);
//	}
//	
//	template<class TSelf, class... TArgs>
//	constexpr auto IsValidIndex(this TSelf& self, TArgs&&... args) requires
//		requires
//	{
//		{ Array(&self).IsValidIndex(std::declval<TArgs>()...) };
//	}
//	{
//		return Array(&self).IsValidIndex(std::forward<TArgs>(args)...);
//	}
//};

export namespace Linq
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