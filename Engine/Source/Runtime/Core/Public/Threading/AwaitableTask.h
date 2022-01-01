// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ContinuousTask.h"
#include "NonCopyable.h"
#include "VirtualDtor.h"
#include <coroutine>

template<class T, template<class> class TTask>
struct AwaitableTask : public NonCopyable
{
	std::vector<std::unique_ptr<VirtualDtor>> ConsumedTask;
	std::promise<T> Promise;
	ContinuousTask<void> Task;
	std::function<void(T)> CompletionTask;

	AwaitableTask()
	{
	}

	~AwaitableTask()
	{
	}

	template<class... U>
	void Consume(U... Object)
	{
		struct Disposable : public VirtualDtor
		{
			std::tuple<U...> Storage;
			Disposable(U... Objects) : Storage(std::make_tuple(std::move(Objects)...))
			{
			}
		};

		ConsumedTask.emplace_back(std::make_unique<Disposable>(std::move(Object)...));
	}

	TTask<T> get_return_object()
	{
		return TTask<T>(*this);
	}

	void return_value(T ReturnValue)
	{
		if constexpr (std::same_as<T, void>)
		{
			Promise.set_value();
		}
		else
		{
			Promise.set_value(ReturnValue);
		}

		if (CompletionTask)
		{
			CompletionTask(ReturnValue);
		}
	}

	void unhandled_exception()
	{
	}

	auto initial_suspend()
	{
		return std::suspend_never();
	}

	auto final_suspend() noexcept
	{
		return std::suspend_never();
	}
};