// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Threading/Tasks/Task.h"
#include <memory>

namespace Ayla
{
	class TaskFactory;

	class CORE_API TaskScheduler : public std::enable_shared_from_this<TaskScheduler>
	{
		friend class TaskFactory;

	public:
		static std::shared_ptr<TaskScheduler> GetDefault();
		static std::shared_ptr<TaskScheduler> GetCurrent();
		static std::shared_ptr<TaskScheduler> FromCurrentSynchronizationContext();

	protected:
		virtual void QueueTask(Task<> task) = 0;

		void TryExecuteTask(Task<> task);
	};
}