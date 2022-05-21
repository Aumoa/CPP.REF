// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <utility>
#include <concepts>

namespace libty::inline Core
{
	class String;

	template<class T>
	concept IObject = requires
	{
		{ std::declval<std::remove_pointer_t<T>>().ToString() } -> std::convertible_to<String>;
	};

	template<IObject T>
	T& to_ref(T& ref)
	{
		return ref;
	}

	template<IObject T>
	const T& to_ref(const T& ref)
	{
		return ref;
	}

	template<IObject T>
	T& to_ref(T* ref)
	{
		return *ref;
	}

	template<IObject T>
	const T& to_ref(const T* ref)
	{
		return *ref;
	}
}