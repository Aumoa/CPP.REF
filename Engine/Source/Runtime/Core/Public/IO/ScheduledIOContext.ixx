// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.IO:ScheduledIOContext;

export import Core.Std;
export import Core.System;
export import Core.Threading;

export class IOContext;

export class CORE_API ScheduledIOContext
{
	ScheduledIOContext(const ScheduledIOContext&) = delete;

private:
	using Clock_t = std::chrono::steady_clock;
	using Work_t = std::function<void(size_t, int32)>;

	Spinlock Lock;
	SpinlockConditionVariable Cval;
	std::multimap<Clock_t::time_point, Work_t> Works;
	std::atomic<bool> bRunning = true;
	std::atomic<size_t> Workers;

public:
	ScheduledIOContext() noexcept;

	size_t Run(IOContext& Context);
	void Stop(IOContext& Context);

	bool Post(std::chrono::nanoseconds InDur, Work_t InWork);
};