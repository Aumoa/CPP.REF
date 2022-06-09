// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Logging/Log.h"
#include "Logging/LogCategory.h"
#include "Logging/LogEntry.h"
#include "Threading/Thread.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include <iostream>
#include <future>

namespace
{
	static Thread sWorker;
	static bool sRunning;
	static Spinlock sLock;
	static SpinlockConditionVariable sCondVar;
	static std::vector<LogEntry> sEntries;
	static std::vector<std::function<void()>> sActions;
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
	sCondVar.NotifyOne();
	lock.unlock();

	sWorker.JoinAsync().Wait();
}

void Log::Print(const LogCategory& logCategory, ELogLevel logLevel, const String& message)
{
	std::unique_lock lock(sLock);
	sEntries.emplace_back() =
	{
		.Category = &logCategory,
		.LogThread = Thread::GetCurrentThread(),
		.Level = logLevel,
		.Message = message
	};
	sCondVar.NotifyOne();
}

void Log::FlushAll()
{
	std::promise<void> promise;
	std::future<void> future = promise.get_future();

	std::unique_lock lock(sLock);
	sActions.emplace_back([&]()
	{
		promise.set_value();
	});
	sCondVar.NotifyOne();

	lock.unlock();
	future.wait();
}

void Log::_Worker()
{
	std::vector<LogEntry> entries;
	std::vector<std::function<void()>> actions;

	static auto levelToString = [](ELogLevel level) -> String
	{
		switch (level)
		{
		case ELogLevel::Debug:
			return TEXT("dbug");
		case ELogLevel::Info:
			return TEXT("info");
		case ELogLevel::Warning:
			return TEXT("warn");
		case ELogLevel::Error:
			return TEXT("erro");
		default:
			return TEXT("ftal");
		}
	};

	while (sRunning || !sEntries.empty() || !sActions.empty())
	{
		std::unique_lock lock(sLock);
		std::swap(entries, sEntries);
		std::swap(actions, sActions);
		lock.unlock();

		for (auto& entry : entries)
		{
			String threadName = entry.LogThread.GetFriendlyName();
			if (!threadName)
			{
				threadName = String::Format(TEXT("[Thread {}]"), entry.LogThread.GetThreadId());
			}

			String formatted = String::Format(TEXT("{0}: {1}: {2}\n      {3}"), levelToString(entry.Level), threadName, entry.Category->GetCategoryName(), entry.Message);
			std::wcout << (std::wstring_view)formatted << std::endl;
		}

		for (auto& action : actions)
		{
			action();
		}

		entries.clear();
		actions.clear();
		lock.lock();
		sCondVar.Wait(lock, []()
		{
			return !sEntries.empty() || !sRunning || !sActions.empty();
		});
	}
}