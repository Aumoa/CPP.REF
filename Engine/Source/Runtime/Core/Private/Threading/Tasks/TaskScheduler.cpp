// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Tasks/TaskScheduler.h"
#include "Threading/Tasks/ThreadPoolTaskScheduler.h"
#include "Threading/SynchronizationContext.h"
#include <thread>

namespace Ayla
{
	thread_local std::shared_ptr<TaskScheduler> g_CurrentTaskScheduler;

	std::shared_ptr<TaskScheduler> TaskScheduler::GetDefault()
	{
		static thread_local auto s_DefaultScheduler = std::make_shared<ThreadPoolTaskScheduler>();
		return s_DefaultScheduler;
	}

	std::shared_ptr<TaskScheduler> TaskScheduler::GetCurrent()
	{
		return g_CurrentTaskScheduler;
	}

	std::shared_ptr<TaskScheduler> TaskScheduler::FromCurrentSynchronizationContext()
	{
		auto current = SynchronizationContext::GetCurrent();
		if (current)
		{
			return current->GetTaskScheduler();
		}
		else
		{
			return nullptr;
		}
	}

	void TaskScheduler::TryExecuteTask(Task<> task)
	{
		auto shared = task.GetShared();
		check(shared->m_Scheduled);
		g_CurrentTaskScheduler = shared_from_this();
		std::exchange(shared->m_Scheduled, {})();
		g_CurrentTaskScheduler = nullptr;
	}

	void TaskScheduler::QueueTaskOnDedicatedThread(Task<> task)
	{
		// Create a dedicated thread for long-running tasks
		// This bypasses the ThreadPool to avoid thread starvation
		std::thread dedicatedThread([this, self = shared_from_this(), task = std::move(task)]() mutable
		{
			TryExecuteTask(std::move(task));
		});

		// Detach the thread so it runs independently
		dedicatedThread.detach();
	}
}