// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/TaskScheduler.h"

namespace Ayla
{
	class ThreadPoolTaskScheduler : public TaskScheduler
	{
	protected:
		virtual void QueueTask(Task<> task) override;
	};
}