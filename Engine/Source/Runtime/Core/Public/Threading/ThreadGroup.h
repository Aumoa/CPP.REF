// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include <chrono>
#include <thread>
#include <functional>
#include <string_view>
#include <stop_token>
#include <queue>

namespace libty::inline Core
{
	class CORE_API ThreadGroup
	{
		ThreadGroup(const ThreadGroup&) = delete;
		ThreadGroup& operator =(const ThreadGroup&) = delete;

	private:
		class ThreadGroupSuspendToken;
		using clock = std::chrono::steady_clock;

		struct ImmediateWork
		{
			std::function<void()> Body;
		};

		struct DelayedWork
		{
			clock::time_point StartsWith;
			std::function<void()> Body;
		};

		template<class TWork>
		struct WorkQueue
		{
			Spinlock lock;
			std::queue<TWork> queue;
			SpinlockConditionVariable cv;

			void enqueue(TWork&& work)
			{
				queue.emplace(std::move(work));
				cv.NotifyOne();
			}

			TWork dequeue()
			{
				auto work = std::move(queue.front());
				queue.pop();
				return std::move(work);
			}
		};

	private:
		std::vector<std::jthread> _threads;

		WorkQueue<ImmediateWork> _immQueue;
		WorkQueue<DelayedWork> _delQueue;

		ThreadGroupSuspendToken* _suspendToken = nullptr;
		std::wstring _groupName;

	public:
		ThreadGroup(std::wstring_view groupName, size_t numThreads = 0);
		virtual ~ThreadGroup() noexcept;

		void Run(std::function<void()> body);
		void Delay(std::chrono::milliseconds timeout, std::function<void()> body);

	private:
		void Worker(size_t index, std::stop_token cancellationToken);
		void Timer(std::stop_token cancellationToken);
	};
}