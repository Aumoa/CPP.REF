// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

ScheduledIOContext::ScheduledIOContext() noexcept
{
}

size_t ScheduledIOContext::Run(IOContext& Context)
{
	if (bRunning == false)
	{
		return 0;
	}

	++Workers;
	size_t Count = 0;

	while (bRunning)
	{
		std::unique_lock ScopedLock(Lock);
		std::optional<Clock_t::time_point> Wait;

		while (Works.size() > 0)
		{
			auto Now = Clock_t::now();
			auto It = Works.begin();
			if (It->first <= Now)
			{
				Context.Post(std::move(It->second));
				Works.erase(It);
				++Count;
			}
			else
			{
				Wait = It->first;
				break;
			}
		}

		if (Wait.has_value())
		{
			Cval.WaitUntil(ScopedLock, *Wait);
		}
		else
		{
			Cval.Wait(ScopedLock);
		}
	}

	--Workers;
	return Count;
}

void ScheduledIOContext::Stop(IOContext& Context)
{
	bRunning = false;

	std::unique_lock ScopedLock(Lock);
	Cval.NotifyAll();
	ScopedLock.unlock();

	while (Workers)
	{
		for (size_t i = 0; i < 1024; ++i)
		{
			PlatformMisc::YieldProcessor();
		}
	}

	for (auto& [_, Work] : Works)
	{
		Context.Post(std::move(Work));
	}

	Works.clear();
}

bool ScheduledIOContext::Post(std::chrono::nanoseconds InDur, Work_t InWork)
{
	if (bRunning == false)
	{
		return false;
	}

	auto StartsWith = Clock_t::now() + InDur;
	std::unique_lock ScopedLock(Lock);
	Works.emplace(StartsWith, std::move(InWork));
	Cval.NotifyOne();
	return true;
}