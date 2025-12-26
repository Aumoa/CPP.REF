// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Tasks/TaskFactory.h"
#include "Threading/Tasks/TaskScheduler.h"

namespace Ayla
{
	TaskFactory::TaskFactory(std::shared_ptr<TaskScheduler> scheduler)
		: m_Scheduler(std::move(scheduler))
	{
	}

	TaskFactory::~TaskFactory() noexcept
	{
	}

	void TaskFactory::QueueTask(Task<> task)
	{
		m_Scheduler->QueueTask(std::move(task));
	}
}