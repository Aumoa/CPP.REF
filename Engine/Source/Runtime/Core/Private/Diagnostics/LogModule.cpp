// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "Diagnostics/LogModule.h"
#include "Diagnostics/LogEntry.h"
#include "Misc/DateTime.h"
#include "Misc/String.h"
#include "Misc/PlatformMacros.h"
#include "Threading/Thread.h"
#include <filesystem>
#include <chrono>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <syncstream>

#if PLATFORM_WINDOWS
#include <Windows.h>
#endif

using namespace libty;

static LogModule* gModule;

struct LogModule::Storage
{
	std::mutex Lock;
	std::queue<LogEntry> Works;
	std::condition_variable Cond;
};

LogModule::LogModule(std::wstring_view ModuleName, size_t QueueSize)
	: ModuleName(ModuleName)
{
	checkf(gModule == nullptr, L"Module is already initialized.");
	gModule = this;
	Impl = new Storage();
}

LogModule::~LogModule()
{
	Shutdown();
}

void LogModule::RunTask()
{
	using namespace std::filesystem;

	path Directory = L"Saved/Logs";
	path LogPath = String::Format(L"{}.log", ModuleName);

	if (!exists(Directory))
	{
		create_directories(Directory);
	}
	else if (exists(Directory / LogPath))
	{
		auto BackupTime = DateTime::Now().ToString<libty::DateTimeFormat::File>();
		path BackupPath = String::ReplaceAll(String::Format(L"{}_{}.log", ModuleName, BackupTime), L":", L"-");
		rename(Directory / LogPath, Directory / BackupPath);
	}

	LogFile.open(Directory / LogPath, std::ios::trunc | std::ios::out);
	checkf(LogFile.is_open(), L"Couldn't open log file.");

	bRunning = true;
	WorkerThread = Thread::CreateThread(L"[Log Module]", std::bind(&LogModule::Worker, this));
}

void LogModule::Shutdown() noexcept
{
	if (bRunning)
	{
		Logged.Clear();

		gModule = nullptr;
		bRunning = false;

		Impl->Cond.notify_all();
		WorkerThread->Join();
		LogFile.close();

		delete Impl;
		Impl = nullptr;
	}
}

void LogModule::EnqueueLogMessage(LogEntry&& entry)
{
	std::unique_lock Lock(Impl->Lock);
	Impl->Works.emplace(std::move(entry).Generate());
	Impl->Cond.notify_one();
}

bool LogModule::IsRunning()
{
	return bRunning;
}

LogModule* LogModule::Get()
{
	return gModule;
}

void LogModule::Worker()
{
	using namespace std::literals;
	using namespace std::chrono;

	steady_clock::time_point Curr = steady_clock::now();

	while (bRunning)
	{
		std::unique_lock Lock(Impl->Lock);
		Impl->Cond.wait(Lock, [this]() { return Impl->Works.size() > 0; });

		std::queue<LogEntry> Works;
		std::swap(Works, Impl->Works);

		Lock.unlock();

		// Write messages.
		while (!Works.empty())
		{
			auto& Entry = Works.front();
			LogFile << String::AsMultibyte(Entry.ToString(true)) << std::endl;
			Logged.Broadcast(Entry);
			if (Entry.Category->GetArguments().bLogToConsole)
			{
				std::wosyncstream(std::wcout) << Entry.ToString() << std::endl;
			}

			if (Entry.Category->GetArguments().bLogToDebugger)
			{
#if PLATFORM_WINDOWS
				// Log to Visual Studio Output Console.
				OutputDebugStringW((std::wstring(Entry.ToString(true)) + L"\n"s).c_str());
#endif
			}
			Works.pop();
		}

		LogFile.flush();
	}
}