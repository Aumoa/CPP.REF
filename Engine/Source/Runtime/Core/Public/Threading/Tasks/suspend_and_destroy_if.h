// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <coroutine>

class suspend_and_destroy_if
{
	bool _suspendAndDestroy;

public:
	inline constexpr suspend_and_destroy_if(bool suspendAndDestroy) noexcept
		: _suspendAndDestroy(suspendAndDestroy)
	{
	}

	inline constexpr bool await_ready() const noexcept
	{
		return !_suspendAndDestroy;
	}

	void await_suspend(std::coroutine_handle<> coro) const noexcept
	{
		if (_suspendAndDestroy)
		{
			coro.destroy();
		}
	}

	inline constexpr void await_resume() const noexcept
	{
	}

	inline explicit constexpr operator bool() const noexcept
	{
		return _suspendAndDestroy;
	}
};