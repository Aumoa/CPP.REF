// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/SharedTask.h"
#include <memory>

namespace Ayla
{
	template<class T>
	class ConfiguredTaskAwaiter
	{
		const std::shared_ptr<SharedTask<T>> m_Task;
		const bool m_ContinueOnCapturedContext;

	public:
		ConfiguredTaskAwaiter(std::shared_ptr<SharedTask<T>> task, bool continueOnCapturedContext) noexcept
			: m_Task(std::move(task))
			, m_ContinueOnCapturedContext(continueOnCapturedContext)
		{
		}

		bool await_ready() const noexcept
		{
			return m_Task->IsCompleted();
		}

		template<class CoroutineHandle>
		void await_suspend(CoroutineHandle&& coro) const noexcept
		{
			m_Task->ContinueWith([c = std::forward<CoroutineHandle>(coro)]()
				{
					c.resume();
				}, m_ContinueOnCapturedContext);
		}

		decltype(auto) await_resume() const
		{
			return m_Task->GetResult();
		}
	};
}