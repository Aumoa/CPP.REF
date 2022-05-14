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
		LogModule(String moduleName);
		~LogModule() noexcept;

		Task<> StartAsync(std::stop_token cancellationToken = {});
		Task<> StopAsync(std::stop_token cancellationToken = {});

		inline void EnqueueLogMessage(LogEntry&& entry)
		{
			this->EnqueueEntry(std::move(entry).Generate());
		}

		inline void EnqueueLogAction(std::function<void()> action)
		{
			this->EnqueueEntry(std::move(action));
		}

		inline Task<> FlushAsync()
		{
			auto tcs = TaskCompletionSource<>::Create();
			std::function<void()> waitingAction = [tcs]() mutable { tcs.SetResult(); };
			this->EnqueueEntry(std::move(waitingAction));
			return tcs.GetTask();
		}

		bool IsRunning();

	public:
		static inline LogModule* Get() noexcept { return sInstance; }

	public:
		DECLARE_MULTICAST_EVENT(LoggedEvent, const LogEntry&);
		LoggedEvent Logged;

	private:
		void EnqueueEntry(Variant_t&& entry);
		void Worker(std::stop_token cancellationToken);
	};
}