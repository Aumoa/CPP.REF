// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/DeferredTaskRunner.h"

std::vector<std::future<void>> DeferredTaskRunner::Runners;
std::vector<std::future<void>> DeferredTaskRunner::Runners_Back;

void DeferredTaskRunner::Run()
{
	std::swap(Runners, Runners_Back);
	for (auto& Runner : Runners)
	{
		Runner.get();
	}
	Runners.clear();
}