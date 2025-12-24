// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Tasks/ThreadPoolTaskScheduler.h"

namespace Ayla
{
	void ThreadPoolTaskScheduler::QueueTask(Task<> task)
	{
		ThreadPool::QueueUserWorkItem([this, task = std::move(task)]() mutable
		{
			TryExecuteTask(std::move(task));
		});
	}
}