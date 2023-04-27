// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IComparable;

export import <utility>;
export import <compare>;
export import <concepts>;

export template<class T, class U>
concept IComparable = requires
{
	{ std::declval<T>().CompareTo(std::declval<U>()) } -> std::convertible_to<std::strong_ordering>;
};