// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomTaskBase.h"
#include "DeferredTask.h"

template<class T = void>
class ContinuousTask : public CustomTaskBase<T>
{
	using MyAwaiter = typename CustomTaskBase<T>::MyAwaiter;

public:
	ContinuousTask()
	{
	}

	ContinuousTask(ContinuousTask&& Rhs) : CustomTaskBase<T>(std::move(Rhs))
	{
	}

private:
	ContinuousTask(std::shared_ptr<MyAwaiter> Awaiter) : CustomTaskBase<T>(std::move(Awaiter))
	{
	}

private:
	static std::future<void>& GetTaskPoolAddress()
	{
		static thread_local std::vector<std::future<void>> TaskPool;
		static thread_local std::vector<size_t> TaskPoolReserved;

		static constexpr const std::chrono::milliseconds Time_GetStatus = std::chrono::milliseconds(0);
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		for (size_t i = 0; i < TaskPool.size(); ++i)
		{
			auto& Task = TaskPool[i];
			if (auto Status = Task.wait_for(Time_GetStatus); Status == std::future_status::ready)
			{
				Task.get();
				TaskPoolReserved.emplace_back(i);
			}
		}

		if (TaskPoolReserved.size())
		{
			size_t Id = TaskPoolReserved.back();
			TaskPoolReserved.erase(TaskPoolReserved.end() - 1);
			return TaskPool[Id];
		}
		else
		{
			return TaskPool.emplace_back();
		}
	}

public:
	template<class _Fn, bool bFallbackToDeferred = true>
	static ContinuousTask Run(_Fn&& Body)
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		GetTaskPoolAddress() = std::async(std::launch::async, [Awaiter, Body = std::move(Body)]() mutable -> void
		{
			if constexpr (std::same_as<T, void>)
			{
				Body();

				if (bFallbackToDeferred)
				{
					DeferredTask<void>::Run([Awaiter = std::move(Awaiter)]()
					{
						Awaiter->SetValue();
					});
				}
				else
				{
					Awaiter->SetValue();
				}
			}
			else
			{
				T ReturnValue = Body();

				if (bFallbackToDeferred)
				{
					DeferredTask<void>::Run([ReturnValue = std::move(ReturnValue), Awaiter = std::move(Awaiter)]() mutable
					{
						Awaiter->SetValue(std::move(ReturnValue));
					});
				}
				else
				{
					Awaiter->SetValue(std::move(ReturnValue));
				}
			}

			Awaiter.reset();
		});

		return ContinuousTask(std::move(Awaiter));
	}

	static Task<> YieldTask() requires std::same_as<T, void>
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		GetTaskPoolAddress() = std::async(std::launch::async, [Awaiter]() mutable -> void
		{
			Awaiter->SetValue();
		});

		return ContinuousTask(std::move(Awaiter));
	}
};