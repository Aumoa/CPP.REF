// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API Parallel : public AbstractClass
{
	static size_t ThreadCount;

public:
	static void For(size_t Count, std::function<void(size_t ThreadIdx, size_t ItemIdx)> Body, size_t NumForceThreads = 0);
	static void ForEach(size_t Count, std::function<void(size_t ThreadIdx, size_t ItemIdx)> Body, size_t NumForceThreads = 0);
	static void ForEach(std::vector<std::future<void>>& Futures, std::function<void()> PreLaunch, size_t Count, std::function<void(size_t ThreadIdx, size_t ItemIdx)> Body, size_t NumForceThreads = 0);
};