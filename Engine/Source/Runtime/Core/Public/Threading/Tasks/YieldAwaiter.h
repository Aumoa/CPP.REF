// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/ThreadPool.h"

namespace Ayla
{
	class YieldAwaiter
	{
	public:
		bool await_ready() const noexcept
		{
			return false;
		}

		template<class CoroutineHandle>
		void await_suspend(CoroutineHandle&& coro) const noexcept
		{
			ThreadPool::QueueUserWorkItem([c = std::forward<CoroutineHandle>(coro)]()
			{
				c.resume();
			});
		}

		void await_resume() const
		{
		}
	};
}