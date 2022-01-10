// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <future>

class CORE_API DeferredTaskRunner
{
	DeferredTaskRunner() = delete;

private:
	static std::vector<std::future<void>> Runners;
	static std::vector<std::future<void>> Runners_Back;

public:
	template<class DeferredAwaiter>
	static void RegisterRunner(DeferredAwaiter* Awaiter) requires requires
	{
		{ Awaiter->GetRunner() };
	}
	{
		Runners_Back.emplace_back(Awaiter->GetRunner());
	}

	static void Run();
};