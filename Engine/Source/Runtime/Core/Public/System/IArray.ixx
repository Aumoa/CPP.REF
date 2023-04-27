// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IArray;

export import :IEnumerable;

export template<class T, class U>
concept IArray = IEnumerable<T, U> && requires (const T& Val)
{
	{ Val[std::declval<size_t>()] } -> std::convertible_to<U>;
	{ std::size(Val) };
	{ std::begin(Val) < std::end(Val) } -> std::convertible_to<bool>;
	{ std::begin(Val) + std::declval<size_t>() };
	{ std::end(Val) - std::begin(Val) };
};