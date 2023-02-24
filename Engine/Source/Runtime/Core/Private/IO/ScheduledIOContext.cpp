// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IO/ScheduledIOContext.h"
#include "IO/IOContext.h"
#include <optional>
#include <mutex>

ScheduledIOContext::ScheduledIOContext() noexcept
{
}

size_t ScheduledIOContext::Run(IOContext& io)
{
	if (_running == false)
	{
		return 0;
	}

	++_workers;
	size_t cnt = 0;

	while (_running)
	{
		std::unique_lock lock(_lock);
		std::optional<_clock::time_point> wait;

		while (_works.size() > 0)
		{
			auto now = _clock::now();
			auto it = _works.begin();
			if (it->first <= now)
			{
				io.Post(std::move(it->second));
				_works.erase(it);
				++cnt;
			}
			else
			{
				wait = it->first;
				break;
			}
		}

		if (wait.has_value())
		{
			_cv.WaitUntil(lock, *wait);
		}
		else
		{
			_cv.Wait(lock);
		}
	}

	--_workers;
	return cnt;
}

void ScheduledIOContext::Stop(IOContext& io)
{
	_running = false;

	std::unique_lock lock(_lock);
	_cv.NotifyAll();
	lock.unlock();

	while (_workers)
	{
		for (size_t i = 0; i < 1024; ++i)
		{
			PlatformMisc::YieldProcessor();
		}
	}

	for (auto& [_, work] : _works)
	{
		io.Post(std::move(work));
	}

	_works.clear();
}

bool ScheduledIOContext::Post(std::chrono::nanoseconds dur, _work_t work)
{
	if (_running == false)
	{
		return false;
	}

	auto startsWith = _clock::now() + dur;
	std::unique_lock lock(_lock);
	_works.emplace(startsWith, std::move(work));
	_cv.NotifyOne();
	return true;
}