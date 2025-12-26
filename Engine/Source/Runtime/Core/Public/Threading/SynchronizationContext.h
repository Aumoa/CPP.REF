// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include <functional>
#include <mutex>
#include <memory>

namespace Ayla
{
	class TaskScheduler;

	class CORE_API SynchronizationContext : public std::enable_shared_from_this<SynchronizationContext>
	{
	public:
		template<class TBody>
#if __cpp_lib_move_only_function
		using function_t = std::move_only_function<TBody>;
#else
		using function_t = std::function<TBody>;
#endif

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