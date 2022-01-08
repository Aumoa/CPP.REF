// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <future>

template<size_t Number = 0>
class DeferredTaskRunner
{
	DeferredTaskRunner() = delete;

private:
	inline static std::vector<std::future<void>> Runners;
	inline static std::vector<std::future<void>> Runners_Back;

public:
	template<class DeferredAwaiter>
	static void RegisterRunner(DeferredAwaiter* Awaiter) requires requires
	{
		{ Awaiter->GetRunner() };
	}
	{
		Runners_Back.emplace_back(Awaiter->GetRunner());
	}

	static void Run()
	{
		std::swap(Runners, Runners_Back);
		for (auto& Runner : Runners)
		{
			Runner.get();
		}
		Runners.clear();
	}
};