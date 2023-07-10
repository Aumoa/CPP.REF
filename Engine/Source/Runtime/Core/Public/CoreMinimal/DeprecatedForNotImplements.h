// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

template<class TOwningClass, class... TArgs>
struct std::coroutine_traits<void, TOwningClass, TArgs...>
{
	using promise_type = ::async_void_promise_type;
};

template<class... TArgs>
struct std::coroutine_traits<void, TArgs...>
{
	using promise_type = ::async_void_promise_type;
};