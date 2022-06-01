// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Generic/IArray.h"
#include <vector>
#include <future>
#include <thread>

class Parallel
{
	Parallel() = delete;
	static const inline size_t ThreadCount = std::thread::hardware_concurrency();

public:
	template<class TContainer, class TWorker>
	static void For(const TContainer& list, TWorker&& body) requires
		IArray<TContainer, EnumerableItem_t<TContainer>> &&
		(std::invocable<TWorker, size_t, EnumerableItem_t<TContainer>> || std::invocable<TWorker, EnumerableItem_t<TContainer>>)
	{
		using Item_t = EnumerableItem_t<TContainer>;

		std::atomic<size_t> seek = 0;
		std::vector<std::future<void>> futures(ThreadCount);

		// Create threads.
		for (size_t i = 0; i < ThreadCount; ++i)
		{
			futures[i] = std::async([&seek, &body, list, count = std::size(list), i]()
			{
				while (true)
				{
					size_t myidx = seek++;
					if (myidx >= count)
					{
						break;
					}

					if constexpr (std::invocable<TWorker, size_t, Item_t>)
					{
						body(i, list[myidx]);
					}
					else if constexpr (std::invocable<TWorker, Item_t>)
					{
						body(list[myidx]);
					}
				}
			});
		}

		// Waiting all threads.
		for (size_t i = 0; i < ThreadCount; ++i)
		{
			futures[i].get();
		}
	}

	template<class TWorker>
	static void ForEach(size_t size, TWorker&& body, size_t threads = ThreadCount) requires
		(std::invocable<TWorker, size_t, size_t, size_t> || std::invocable<TWorker, size_t, size_t>)
	{
		const size_t ItemPerThread = (size - 1) / threads + 1;
		std::vector<std::future<void>> futures(threads);

		// Create threads.
		for (size_t i = 0; i < threads; ++i)
		{
			const size_t StartIndex = i * ItemPerThread;
			const size_t EndIndex = std::min((i + 1) * ItemPerThread, size);

			if constexpr (std::invocable<TWorker, size_t, size_t, size_t>)
			{
				futures[i] = std::async(std::launch::async, std::bind(body, i, StartIndex, EndIndex));
			}
			else if constexpr (std::invocable<TWorker, size_t, size_t>)
			{
				futures[i] = std::async(std::launch::async, std::bind(body, StartIndex, EndIndex));
			}
		}

		// Waiting all threads.
		for (size_t i = 0; i < threads; ++i)
		{
			using namespace std;
			while (futures[i].wait_for(1ms) != std::future_status::ready);
		}
	}
};