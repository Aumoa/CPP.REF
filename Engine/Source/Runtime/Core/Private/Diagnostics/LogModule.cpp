// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "Diagnostics/LogModule.h"
#include "Misc/DateTime.h"
#include "Misc/String.h"
#include "Threading/Thread.h"
#include <filesystem>
#include <chrono>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

static LogModule* gModule;

struct LogModule::Storage
{
	std::mutex Lock;
	std::queue<std::wstring> Works;
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
	path LogPath = std::format(L"{}.log", ModuleName);

	if (!exists(Directory))
	{
		create_directories(Directory);
	}
	else if (exists(Directory / LogPath))
	{
		auto BackupTime = DateTime::Now().ToString<libty::DateTimeFormat::File>();
		path BackupPath = String::ReplaceAll(std::format(L"{}_{}.log", ModuleName, BackupTime), L":", L"-");
		rename(Directory / LogPath, Directory / BackupPath);
	}

	LogFile.open(Directory / LogPath, std::ios::trunc | std::ios::out);
	checkf(LogFile.is_open(), L"Couldn't open log file.");

	bRunning = true;
	WorkerThread = Thread::CreateThread(L"[Log Module]", std::bind(&LogModule::Worker, this));
}

void LogModule::Shutdown()
{
	if (bRunning)
	{
		Logged.Clear();

		gModule = nullptr;
		bRunning = false;

		WorkerThread->Join();
		LogFile.close();

		delete Impl;
		Impl = nullptr;
	}
}

void LogModule::EnqueueLogMessage(std::wstring_view Message)
{
	std::unique_lock Lock(Impl->Lock);
	Logged.Broadcast(Message);
	Impl->Works.emplace(std::wstring(Message));
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
		Impl->Cond.wait(Lock);

		std::queue<std::wstring> Works;
		std::swap(Works, Impl->Works);

		Lock.unlock();

		// Write messages.
		while (!Works.empty())
		{
			auto& Wstr = Works.front();
			LogFile << String::AsMultibyte(Wstr) << std::endl;
			Works.pop();
		}

		LogFile.flush();
	}
}