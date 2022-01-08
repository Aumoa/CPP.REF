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
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		Awaiter->SetRunner(std::async(std::launch::async, [Awaiter, Body = std::move(Body)]() mutable -> void
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

		return ContinuousTask(std::move(Awaiter));
	}
};