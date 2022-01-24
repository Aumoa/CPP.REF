// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomTaskBase.h"
#include "DeferredTaskRunner.h"

template<class T = void>
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

		DeferredTaskRunner::RegisterRunner([Awaiter, Body = std::move(Body)]() mutable -> void
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

		return DeferredTask(std::move(Awaiter));
	}

	static Task<> YieldTask() requires std::same_as<T, void>
	{
		std::shared_ptr Awaiter = std::make_shared<MyAwaiter>();

		DeferredTaskRunner::RegisterRunner([Awaiter]() -> void
		{
			Awaiter->SetValue();
		});

		return DeferredTask(std::move(Awaiter));
	}

	template<class TDuration>
	static Task<> Delay(TDuration Timespan) requires requires
	{
		{ (std::chrono::steady_clock::now() - std::chrono::steady_clock::now()) >= Timespan } -> std::convertible_to<bool>;
	}
	{
		using namespace std::chrono;

		auto StartTime = steady_clock::now();
		while (true)
		{
			auto CurrTime = steady_clock::now();
			auto Dur = CurrTime - StartTime;
			if (Dur >= Timespan)
			{
				break;
			}

			co_await YieldTask();
		}
	}

	static Task<> Delay(int32 Milliseconds)
	{
		return Delay(std::chrono::milliseconds(Milliseconds));
	}
};