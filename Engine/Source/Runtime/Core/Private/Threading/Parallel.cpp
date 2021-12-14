// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Threading/Parallel.h"
#include <future>

size_t Parallel::ThreadCount = 16;

void Parallel::For(size_t Count, std::function<void(size_t, size_t)> Body, size_t NumForceThreads)
{
	size_t NumThreads = NumForceThreads == 0 ? ThreadCount : NumForceThreads;
	std::atomic<size_t> Seek = 0;

	std::vector<std::future<void>> Futures;
	Futures.reserve(NumThreads);

	// Create threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		Futures.emplace_back(std::async([&Seek, &Body, Count, i]()
		{
			while (true)
			{
				size_t Myidx = Seek++;
				if (Myidx >= Count)
				{
					break;
				}

				Body(i, Myidx);
			}
		}));
	}

	// Waiting all threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		Futures[i].get();
	}
}

void Parallel::ForEach(size_t Count, std::function<void(size_t, size_t)> Body, size_t NumForceThreads)
{
	std::vector<std::future<void>> Futures;
	ForEach(Futures, nullptr, Count, Body, NumForceThreads);
}

void Parallel::ForEach(std::vector<std::future<void>>& Futures, std::function<void()> PreLaunch, size_t Count, std::function<void(size_t, size_t)> Body, size_t NumForceThreads)
{
	size_t NumThreads = NumForceThreads == 0 ? ThreadCount : NumForceThreads;
	size_t ItemPerThread = (Count - 1) / NumThreads + 1;
	Futures.reserve(NumThreads);

	std::promise<void> AsyncLauncher;
	std::shared_future<void> IsReady = AsyncLauncher.get_future();

	// Create threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		size_t StartIndex = i * ItemPerThread;
		size_t EndIndex = std::min((i + 1) * ItemPerThread, Count);

		Futures.emplace_back(std::async(std::launch::async, [&Body, StartIndex, EndIndex, ThreadIdx = i, &IsReady]()
		{
			IsReady.get();

			for (size_t i = StartIndex; i < EndIndex; ++i)
			{
				Body(ThreadIdx, i);
			}
		}));
	}

	if (PreLaunch)
	{
		PreLaunch();
	}

	AsyncLauncher.set_value();

	// Waiting all threads.
	for (size_t i = 0; i < NumThreads; ++i)
	{
		Futures[i].get();
	}
}