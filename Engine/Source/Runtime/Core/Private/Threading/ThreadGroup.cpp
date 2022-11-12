// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/ThreadGroup.h"
#include "Exceptions/InvalidOperationException.h"
#include "Misc/String.h"
#include "Misc/PlatformMisc.h"

ThreadGroup::ThreadGroup(const String& groupName)
	: _groupName(groupName)
{
}

ThreadGroup::~ThreadGroup() noexcept
{
}

void ThreadGroup::CreateThread(Worker_t body)
{
	size_t index = _threads.size();
	_threads.emplace_back([index, body, name = _groupName](std::stop_token cancellationToken)
	{
		PlatformMisc::SetThreadName(String::Format(TEXT("[{} #{}]"), name, index));
		PlatformMisc::GuardedMain([body, index, cancellationToken]()
		{
			body(index, cancellationToken);
			return 0;
		});
	});
}

void ThreadGroup::StopAll()
{
	for (auto& thr : _threads)
	{
		thr.request_stop();
	}
}

void ThreadGroup::JoinAll()
{
	for (auto& thr : _threads)
	{
		thr.join();
	}
}