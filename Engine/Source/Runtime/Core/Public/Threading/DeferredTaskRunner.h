// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <future>

class CORE_API DeferredTaskRunner
{
	DeferredTaskRunner() = delete;

public:
	static void RegisterRunner(std::future<void> Runner);
	static void Run();
};