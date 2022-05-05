// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Delegates/MulticastEvent.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Diagnostics/LogEntry.h"
#include <fstream>
#include <atomic>
#include <variant>

namespace libty::inline Core
{
	class Thread;

	class CORE_API LogModule
	{
		using This = LogModule;
		using Variant_t = std::variant<LogEntry, std::function<void()>>;

	private:
		static LogModule* sInstance;

	private:
		String _name;
		Thread* _thread = nullptr;
		std::stop_source _stopSource;
		std::ofstream _logFile;

		Spinlock _mutex;
		SpinlockConditionVariable _cv;
		std::vector<Variant_t> _entries;

	public:
		LogModule(StringView moduleName);
		~LogModule() noexcept;

		Task<> StartAsync(std::stop_token cancellationToken = {});
		Task<> StopAsync(std::stop_token cancellationToken = {});

		void EnqueueLogMessage(LogEntry&& entry);
		void EnqueueLogAction(std::function<void()> action);
		bool IsRunning();

	public:
		static inline LogModule* Get() noexcept { return sInstance; }

	public:
		DECLARE_MULTICAST_EVENT(LoggedEvent, const LogEntry&);
		LoggedEvent Logged;

	private:
		void Worker(std::stop_token cancellationToken);
	};
}