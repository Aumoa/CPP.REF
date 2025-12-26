// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/SynchronizationContext.h"
#include "Threading/Tasks/TaskScheduler.h"
#include "Threading/Tasks/SynchronizationContextTaskScheduler.h"

namespace Ayla
{
	thread_local std::weak_ptr<SynchronizationContext> g_Current;

	void SynchronizationContext::SetSynchronizationContext(std::shared_ptr<SynchronizationContext> context)
	{
		g_Current = context;
	}

	std::shared_ptr<SynchronizationContext> SynchronizationContext::GetCurrent()
	{
		return g_Current.lock();
	}

	std::shared_ptr<TaskScheduler> SynchronizationContext::GetTaskScheduler()
	{
		if (!m_TaskScheduler)
		{
			auto lock = std::unique_lock(m_Mutex);
			if (!m_TaskScheduler)
			{
				m_TaskScheduler = std::make_shared<SynchronizationContextTaskScheduler>(weak_from_this());
			}
		}

		return m_TaskScheduler;
	}
}