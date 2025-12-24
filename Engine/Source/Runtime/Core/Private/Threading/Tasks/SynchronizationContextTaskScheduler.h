// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/TaskScheduler.h"

namespace Ayla
{
	class SynchronizationContextTaskScheduler : public TaskScheduler
	{
		std::weak_ptr<SynchronizationContext> m_Context;

	public:
		SynchronizationContextTaskScheduler(std::weak_ptr<SynchronizationContext> context);

	protected:
		virtual void QueueTask(Task<> task) override;
	};
}