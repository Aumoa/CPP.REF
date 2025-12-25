// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Tasks/ThreadPoolTaskScheduler.h"
#include "Threading/Tasks/TaskCreationOptions.h"

namespace Ayla
{
	void ThreadPoolTaskScheduler::QueueTask(Task<> task)
	{
		auto shared = task.GetShared();
		
		// Check for LongRunning option
		if (HasFlag(shared->GetOptions(), TaskCreationOptions::LongRunning))
		{
			// Create dedicated thread for long-running task (bypasses ThreadPool)
			QueueTaskOnDedicatedThread(std::move(task));
		}
		else
		{
			// Use ThreadPool for normal tasks
			ThreadPool::QueueUserWorkItem([this, task = std::move(task)]() mutable
			{
				TryExecuteTask(std::move(task));
			});
		}
	}
}