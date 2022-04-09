// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Delegates/MulticastEvent.h"
#include <atomic>
#include <fstream>
#include <future>

namespace libty::inline Core
{
	class Thread;
}

namespace libty::inline Core
{
	struct LogEntry;

	class CORE_API LogModule
	{
		using This = LogModule;

	private:
		std::wstring ModuleName;
		Thread* WorkerThread = nullptr;
		std::atomic<bool> bRunning;
		std::ofstream LogFile;

		struct Storage;
		Storage* Impl = nullptr;

	public:
		LogModule(std::wstring_view ModuleName, size_t QueueSize = 1024);
		~LogModule() noexcept;

		void RunTask();
		void Shutdown() noexcept;
		void EnqueueLogMessage(LogEntry&& entry);
		bool IsRunning();

		static LogModule* Get();

	public:
		DECLARE_MULTICAST_EVENT(LoggedEvent, const LogEntry&);
		LoggedEvent Logged;

	private:
		void Worker();
	};
}