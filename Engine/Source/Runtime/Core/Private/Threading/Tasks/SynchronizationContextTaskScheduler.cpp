// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Tasks/SynchronizationContextTaskScheduler.h"
#include "Threading/Tasks/TaskCreationOptions.h"
#include "NullReferenceException.h"

namespace Ayla
{
	SynchronizationContextTaskScheduler::SynchronizationContextTaskScheduler(std::weak_ptr<SynchronizationContext> context)
		: m_Context(std::move(context))
	{
	}

	void SynchronizationContextTaskScheduler::QueueTask(Task<> task)
	{
		auto shared = task.GetShared();
		
		// Check for LongRunning option
		// LongRunning tasks bypass SynchronizationContext and create dedicated threads
		if (HasFlag(shared->GetOptions(), TaskCreationOptions::LongRunning))
		{
			QueueTaskOnDedicatedThread(std::move(task));
			return;
		}

		// Normal tasks respect SynchronizationContext
		auto context = m_Context.lock();
		if (context)
		{
			context->Post([this, self = shared_from_this(), task = std::move(task)]() mutable
			{
				TryExecuteTask(std::move(task));
			});
		}
		else
		{
			throw NullReferenceException();
		}
	}
}