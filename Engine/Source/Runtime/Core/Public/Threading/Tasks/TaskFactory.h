// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskScheduler.h"
#include "Threading/Tasks/TaskCreationOptions.h"
#include <memory>

namespace Ayla
{
	class TaskScheduler;

	class CORE_API TaskFactory
	{
	private:
		std::shared_ptr<TaskScheduler> m_Scheduler;

	public:
		TaskFactory(std::shared_ptr<TaskScheduler> scheduler);
		~TaskFactory() noexcept;
		
		template<class TBody>
		auto StartNew(TBody&& continuationBody, std::stop_token cancellationToken = {}) -> Task<std::invoke_result_t<TBody>>
		{
			return StartNew(std::forward<TBody>(continuationBody), TaskCreationOptions::None, cancellationToken);
		}

		template<class TBody>
		auto StartNew(TBody&& continuationBody, TaskCreationOptions creationOptions, std::stop_token cancellationToken = {}) -> Task<std::invoke_result_t<TBody>>
		{
			using U = std::invoke_result_t<TBody>;
			std::shared_ptr task = std::make_shared<SharedTask<U>>(cancellationToken);

			task->m_Options = creationOptions;
			task->m_Scheduled = [sharedTask = task, continuationBody = std::forward<TBody>(continuationBody)]() mutable
			{
				sharedTask->TransitToRunning();

				try
				{
					if constexpr (std::same_as<U, void>)
					{
						continuationBody();
						sharedTask->SetResult();
					}
					else
					{
						U result = continuationBody();
						sharedTask->SetResult(std::move(result));
					}
				}
				catch (...)
				{
					bool b = sharedTask->TrySetException(std::current_exception());
					check(b);
				}
			};

			auto wrapped = Task<U>(std::move(task));

			// Delegate to TaskScheduler - it will handle LongRunning option
			QueueTask(wrapped);
			return wrapped;
		}

	private:
		void QueueTask(Task<> task);
	};

	template<class T>
	std::shared_ptr<TaskFactory> Task<T>::GetFactory()
	{
		static std::shared_ptr<TaskFactory> s_Factory = std::make_shared<TaskFactory>(TaskScheduler::GetDefault());
		return s_Factory;
	}

	template<class T>
	template<class TBody>
	auto Task<T>::Run(TBody&& continuationBody, std::stop_token cancellationToken) -> Task<std::invoke_result_t<TBody>>
	{
		static_assert(std::same_as<T, void>, "Use Task<>::Run instead.");
		return GetFactory()->StartNew(std::forward<TBody>(continuationBody), cancellationToken);
	}
}