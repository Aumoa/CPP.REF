// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <coroutine>

namespace Ayla
{
	class suspend_and_destroy_if
	{
		const bool bSuspendAndDestroy;

	public:
		inline constexpr suspend_and_destroy_if(bool bSuspendAndDestroy) noexcept
			: bSuspendAndDestroy(bSuspendAndDestroy)
		{
		}

		inline constexpr bool await_ready() const noexcept
		{
			return !bSuspendAndDestroy;
		}

		void await_suspend(std::coroutine_handle<> Coro) const noexcept
		{
			if (bSuspendAndDestroy)
			{
				Coro.destroy();
			}
		}

		inline constexpr void await_resume() const noexcept
		{
		}

		inline explicit constexpr operator bool() const noexcept
		{
			return bSuspendAndDestroy;
		}
	};
}