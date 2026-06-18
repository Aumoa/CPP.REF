// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "MoveOnlyFunction.h"
#include <mutex>
#include <memory>

namespace Ayla
{
	class TaskScheduler;

	class CORE_API SynchronizationContext : public std::enable_shared_from_this<SynchronizationContext>
	{
	public:
		template<class TBody>
		using function_t = MoveOnlyFunction<TBody>;

	public:
		virtual void Post(function_t<void()> continuation) = 0;

	public:
		static void SetSynchronizationContext(std::shared_ptr<SynchronizationContext> context);
		static std::shared_ptr<SynchronizationContext> GetCurrent();

	private:
		friend class TaskScheduler;
		std::shared_ptr<TaskScheduler> m_TaskScheduler;
		std::mutex m_Mutex;
		std::shared_ptr<TaskScheduler> GetTaskScheduler();
	};
}
