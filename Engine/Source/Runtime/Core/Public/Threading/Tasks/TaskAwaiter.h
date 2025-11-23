// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/SharedTask.h"
#include <memory>

namespace Ayla
{
	template<class T>
	class TaskAwaiter
	{
	private:
		const std::shared_ptr<SharedTask<T>> m_Task;

	public:
		TaskAwaiter(std::shared_ptr<SharedTask<T>> task)
			: m_Task(std::move(task))
		{
		}

		bool await_ready() const noexcept
		{
			return m_Task->IsCompleted();
		}

		template<class CoroutineHandle>
		void await_suspend(CoroutineHandle&& coro) const noexcept
		{
			constexpr bool kContinueOnCapturedContext = true;
			m_Task->ContinueWith([c = std::forward<CoroutineHandle>(coro)]()
			{
				c.resume();
			}, kContinueOnCapturedContext);
		}

		decltype(auto) await_resume() const
		{
			return m_Task->GetResult();
		}
	};
}