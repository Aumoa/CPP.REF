// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomTaskBase.h"
#include "DeferredTaskRunner.h"

template<class T>
class DeferredTask : public CustomTaskBase<T>
{
	using MyAwaiter = typename CustomTaskBase<T>::MyAwaiter;

public:
	DeferredTask()
	{
	}

	DeferredTask(DeferredTask&& Rhs) : CustomTaskBase<T>(std::move(Rhs))
	{
	}

private:
	DeferredTask(std::shared_ptr<MyAwaiter> Awaiter) : CustomTaskBase<T>(std::move(Awaiter))
	{
	}

public:
	template<size_t RunnerNumber = 0, class _Fn>
	static DeferredTask Run(_Fn&& Body)
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		Awaiter->SetRunner(std::async(std::launch::deferred, [Awaiter, Body = std::move(Body)]() mutable -> void
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
		}));

		DeferredTaskRunner<RunnerNumber>::RegisterRunner(Awaiter.get());
		return DeferredTask(std::move(Awaiter));
	}
};