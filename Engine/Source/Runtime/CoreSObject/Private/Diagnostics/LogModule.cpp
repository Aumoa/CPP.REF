// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/LogModule.h"
#include "Misc/DateTime.h"
#include "Misc/StringUtils.h"
#include "Threading/Thread.h"
#include <filesystem>
#include <chrono>
#include <atomic>
#include <boost/asio/io_service.hpp>

static LogModule* gModule;

struct LogModule::Storage
{
	boost::asio::io_service IO;
	std::atomic<std::size_t> Pending;
	std::vector<std::wstring> Buffers;
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
		auto BackupTime = DateTime<>::Now().ToString();
		path BackupPath = StringUtils::ReplaceAll(std::format(L"{}_{}.log", ModuleName, BackupTime), L":", L"-");
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
	Logged.Broadcast(Message);
	Impl->IO.post([&, Message = std::wstring(Message)]() mutable
	{
		Impl->Buffers.emplace_back(std::move(Message));
	});
	++Impl->Pending;
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
		// Load
		size_t Pending = Impl->Pending;
		size_t Consume = 0;

		Impl->Buffers.clear();
		for (size_t i = 0; i < Pending; ++i)
		{
			// Consume
			Consume += Impl->IO.poll_one();
		}

		// Apply
		Impl->Pending -= Consume;

		// Write messages.
		if (Impl->Buffers.size())
		{
			for (auto& Message : Impl->Buffers)
			{
				LogFile << WCHAR_TO_ANSI(Message) << std::endl;
			}
			LogFile.flush();
		}

		// Waiting for next frame.
		Curr += milliseconds(16);
		std::this_thread::sleep_until(Curr);
	}
}