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
	template<class _Fn>
	static DeferredTask Run(_Fn&& Body)
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		std::future<void> Runner = std::async(std::launch::deferred, [Awaiter, Body = std::move(Body)]() mutable -> void
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
		});

		DeferredTaskRunner::RegisterRunner(std::move(Runner));
		return DeferredTask(std::move(Awaiter));
	}
};