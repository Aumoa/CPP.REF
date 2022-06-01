// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"
#include <stop_token>
#include <functional>

template<class T>
class co_push
{
	co_push() = delete;
	co_push(const co_push&) = delete;

private:
	T _item;

public:
	template<class... TArgs> requires
		std::constructible_from<T, TArgs...>
	co_push(TArgs&&... args) noexcept(noexcept(T(std::forward<TArgs>(args)...)))
		: _item(std::forward<TArgs>(args)...)
	{
	}

	T _Consume() &&
	{
		return std::move(_item);
	}
};

template<class... TArgs> requires
	std::constructible_from<std::stop_token, TArgs...>
co_push(TArgs&&...) -> co_push<std::stop_token>;

template<class... TArgs> requires
	std::constructible_from<std::function<bool()>, TArgs...>
co_push(TArgs&&...) -> co_push<std::stop_token>;