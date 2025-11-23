// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/ThreadPool.h"
#include "Threading/SynchronizationContext.h"

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
			auto continuation = [c = std::forward<CoroutineHandle>(coro)]()
			{
				c.resume();
			};

			if (auto syncContext = SynchronizationContext::GetCurrent())
			{
				syncContext->Post(std::move(continuation));
				return;
			}

			ThreadPool::QueueUserWorkItem(std::move(continuation));
		}

		void await_resume() const
		{
		}
	};
}