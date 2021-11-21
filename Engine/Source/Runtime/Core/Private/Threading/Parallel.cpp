// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/Parallel.h"
#include <future>

size_t Parallel::ThreadCount = 16;

void Parallel::For(size_t Count, std::function<void(size_t)> Body, size_t NumForceThreads)
{
	size_t NumThreads = NumForceThreads == 0 ? ThreadCount : NumForceThreads;
	std::atomic<size_t> Seek = 0;

	std::vector<std::future<void>> Futures;
	Futures.reserve(NumThreads);

	// Create threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		Futures.emplace_back(std::async([&Seek, &Body, Count]()
		{
			while (true)
			{
				size_t Myidx = Seek++;
				if (Myidx >= Count)
				{
					break;
				}

				Body(Myidx);
			}
		}));
	}

	// Waiting all threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		Futures[i].get();
	}
}