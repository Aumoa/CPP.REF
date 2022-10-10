// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Misc/String.h"
#include <chrono>
#include <thread>
#include <functional>
#include <string_view>
#include <stop_token>
#include <map>

class CORE_API ThreadGroup
{
	ThreadGroup(const ThreadGroup&) = delete;
	ThreadGroup& operator =(const ThreadGroup&) = delete;

private:
	using clock = std::chrono::steady_clock;

	template<class TPriority, class TWork>
	struct WorkPriorityQueue
	{
		Spinlock lock;
		std::multimap<TPriority, TWork> queue;
		SpinlockConditionVariable cv;

		inline void enqueue(TPriority&& priority, TWork&& work) noexcept
		{
			queue.emplace(std::move(priority), std::move(work));
			cv.NotifyOne();
		}

		inline const TPriority& priority() const noexcept
		{
			return queue.begin()->first;
		}

		TWork dequeue()
		{
			auto it = queue.begin();
			TWork work = std::move(it->second);
			queue.erase(it);
			return std::move(work);
		}
	};

private:
	String _groupName;
	std::vector<std::jthread> _threads;

	WorkPriorityQueue<int64, std::function<void()>> _immQueue;
	WorkPriorityQueue<clock::time_point, std::function<void()>> _delQueue;

protected:
	using Worker_t = std::function<void(size_t index, std::stop_token)>;

public:
	ThreadGroup(const String& groupName, size_t numThreads = 0, bool timer = true, Worker_t worker = nullptr);
	virtual ~ThreadGroup() noexcept;

	void Run(std::function<void()> body);
	void Delay(std::chrono::milliseconds timeout, std::function<void()> body);

protected:
	void ReadyWorkerThread(size_t index);

private:
	void Worker(size_t index, std::stop_token cancellationToken);
	void Timer(std::stop_token cancellationToken);
};