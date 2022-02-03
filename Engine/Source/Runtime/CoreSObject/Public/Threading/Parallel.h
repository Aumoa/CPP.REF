// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "AbstractClass.h"
#include <functional>
#include <vector>
#include <future>

class CORESOBJECT_API Parallel : public AbstractClass
{
	static size_t ThreadCount;

public:
	static void For(size_t Count, std::function<void(size_t ThreadIdx, size_t ItemIdx)> Body, size_t NumForceThreads = 0);
	static void ForEach(size_t Count, std::function<void(size_t ThreadIdx, size_t Start, size_t End)> Body, size_t NumForceThreads = 0);
	static void ForEach(std::vector<std::future<void>>& Futures, size_t Count, std::function<void(size_t ThreadIdx, size_t Start, size_t End)> Body, size_t NumForceThreads = 0);
};