// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IVector;

export import :IList;

export template<class T, class U>
concept IVector = IList<T, U> && requires
{
	{ std::declval<T>()[std::declval<size_t>()] } -> std::convertible_to<U>;
	{ std::declval<T>().reserve(std::declval<size_t>()) };
};