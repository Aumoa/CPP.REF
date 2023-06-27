// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IEnumerable;

export import Core.Std;

export template<class T, class U>
concept IEnumerable = requires (const T& Val)
{
	{ *std::begin(Val) } -> std::convertible_to<U>;
	{ std::begin(Val) != std::end(Val) } -> std::convertible_to<bool>;
	{ std::advance(std::declval<std::remove_reference_t<decltype(std::begin(Val))>&>(), 1) };
};

export template<class TEnumerable>
using EnumerableItem_t = std::remove_const_t<std::remove_reference_t<decltype(*std::begin(std::declval<TEnumerable>()))>>;