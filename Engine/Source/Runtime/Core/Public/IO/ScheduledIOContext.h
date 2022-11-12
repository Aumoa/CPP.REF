// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include <chrono>
#include <map>
#include <functional>
#include <atomic>

class IOContext;

class CORE_API ScheduledIOContext
{
	ScheduledIOContext(const ScheduledIOContext&) = delete;

private:
	using _clock = std::chrono::steady_clock;
	using _work_t = std::function<void()>;

	Spinlock _lock;
	SpinlockConditionVariable _cv;
	std::multimap<_clock::time_point, _work_t> _works;
	std::atomic<bool> _running = true;
	std::atomic<size_t> _workers;

public:
	ScheduledIOContext() noexcept;

	size_t Run(IOContext& io);
	void Stop(IOContext& io);

	bool Post(std::chrono::nanoseconds dur, std::function<void()> work);
};