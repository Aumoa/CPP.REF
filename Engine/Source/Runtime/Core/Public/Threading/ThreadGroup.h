// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <chrono>
#include <thread>
#include <functional>
#include <string_view>
#include <stop_token>
#include <mutex>
#include <queue>
#include <condition_variable>

class CORE_API ThreadGroup
{
	class ThreadGroupSuspendToken;
	using clock = std::chrono::steady_clock;

	struct Work
	{
		clock::time_point ExpireTime;
		std::function<void()> Body;
	};

private:
	std::vector<std::jthread> _threads;

	std::mutex _lock;
	std::queue<Work> _works;
	std::condition_variable _cv;

	ThreadGroupSuspendToken* _suspendToken = nullptr;
	std::wstring _groupName;

public:
	ThreadGroup(std::wstring_view groupName, size_t numThreads = 0);
	virtual ~ThreadGroup() noexcept;

	void Run(std::function<void()> body);
	void Delay(std::chrono::milliseconds timeout, std::function<void()> body);

private:
	void Worker(size_t index, std::stop_token cancellationToken);
};