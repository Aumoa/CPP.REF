// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Logging/Log.h"
#include "Logging/LogCategory.h"
#include "Logging/LogEntry.h"
#include "Threading/Thread.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include <iostream>

namespace
{
	static Thread sWorker;
	static bool sRunning;
	static Spinlock sLock;
	static SpinlockConditionVariable sCondVar;
	static std::vector<LogEntry> sEntries;
}

void Log::_Initialize()
{
	sRunning = true;
	sWorker = Thread::CreateThread(TEXT("[Log Worker]"), &_Worker);
}

void Log::_Shutdown()
{
	auto lock = std::unique_lock(sLock);
	sRunning = false;
	lock.unlock();

	sWorker.JoinAsync().Wait();
}

void Log::Print(const LogCategory& logCategory, ELogLevel logLevel, const String& message)
{
	std::unique_lock lock(sLock);
	sEntries.emplace_back() =
	{
		.Category = &logCategory,
		.Level = logLevel,
		.Message = message
	};
	sCondVar.NotifyOne();
}

void Log::_Worker()
{
	std::vector<LogEntry> entries;

	while (sRunning)
	{
		std::unique_lock lock(sLock);
		std::swap(entries, sEntries);
		lock.unlock();

		for (auto& entry : entries)
		{
			std::wcout << (std::wstring_view)entry.Message << std::endl;
		}

		entries.clear();
		lock.lock();
		sCondVar.Wait(lock, []()
		{
			return !sEntries.empty() || !sRunning;
		});
	}
}