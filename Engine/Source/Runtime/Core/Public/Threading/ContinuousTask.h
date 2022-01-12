// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomTaskBase.h"

template<class T>
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

public:
	template<class _Fn>
	static ContinuousTask Run(_Fn&& Body)
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

		std::future NewTask = std::async(std::launch::async, [Awaiter, Body = std::move(Body)]() mutable -> void
		{
			if constexpr (std::same_as<T, void>)
			{
				Body();
				Awaiter->SetValue();
			}
			else
			{
				T ReturnValue = Body();
				Awaiter->SetValue(std::move(ReturnValue));
			}

			Awaiter.reset();
		});

		if (TaskPoolReserved.size())
		{
			size_t Id = TaskPoolReserved.back();
			TaskPoolReserved.erase(TaskPoolReserved.end() - 1);
			TaskPool[Id] = std::move(NewTask);
		}
		else
		{
			TaskPool.emplace_back(std::move(NewTask));
		}

		return ContinuousTask(std::move(Awaiter));
	}
};