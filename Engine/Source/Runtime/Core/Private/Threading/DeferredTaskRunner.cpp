// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/DeferredTaskRunner.h"
#include <mutex>

static std::mutex Runners_mutex;
static std::vector<std::future<void>> Runners;
static std::vector<std::future<void>> Runners_Back;

void DeferredTaskRunner::RegisterRunner(std::future<void> Runner)
{
	std::unique_lock Runners_lock(Runners_mutex);
	Runners_Back.emplace_back(std::move(Runner));
}

void DeferredTaskRunner::Run()
{
	{
		std::unique_lock Runners_lock(Runners_mutex);
		std::swap(Runners, Runners_Back);
	}

	for (auto& Runner : Runners)
	{
		Runner.get();
	}
	Runners.clear();
}