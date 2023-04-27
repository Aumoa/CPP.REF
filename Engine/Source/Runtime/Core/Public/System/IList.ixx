// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IList;

export import :IEnumerable;

export template<class T, class U>
concept IList = IEnumerable<T, U> && requires
{
	{ std::size(std::declval<T>()) } -> std::convertible_to<size_t>;
	{ std::declval<T>().emplace_back(std::declval<U>()) };
};