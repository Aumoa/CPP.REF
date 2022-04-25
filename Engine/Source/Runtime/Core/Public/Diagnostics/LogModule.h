// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Delegates/MulticastEvent.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Diagnostics/LogEntry.h"
#include "Misc/SingletonSupports.h"
#include <fstream>
#include <queue>
#include <atomic>

namespace libty::inline Core
{
	class Thread;

	class CORE_API LogModule : public SingletonSupports<LogModule>
	{
		using This = LogModule;

	private:
		std::wstring _name;
		Thread* _thread = nullptr;
		std::stop_source _stopSource;
		std::ofstream _logFile;

		Spinlock _mutex;
		SpinlockConditionVariable _cv;
		std::vector<LogEntry> _entries;

	public:
		LogModule(std::wstring_view moduleName);
		~LogModule() noexcept;

		Task<> StartAsync(std::stop_token cancellationToken = {});
		Task<> StopAsync(std::stop_token cancellationToken = {});

		void EnqueueLogMessage(LogEntry&& entry);
		bool IsRunning();

	public:
		DECLARE_MULTICAST_EVENT(LoggedEvent, const LogEntry&);
		LoggedEvent Logged;

	private:
		void Worker(TaskCompletionSource<>* init, std::stop_token cancellationToken);
	};
}