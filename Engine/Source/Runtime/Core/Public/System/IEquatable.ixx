// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:IEquatable;

export import <utility>;
export import <concepts>;

export template<class T, class U>
concept IEquatable = requires
{
	{ std::declval<T>().Equals(std::declval<U>()) } -> std::convertible_to<bool>;
};