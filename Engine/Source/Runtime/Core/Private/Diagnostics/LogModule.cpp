// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Diagnostics/LogModule.h"
#include "Misc/DateTime.h"
#include "Threading/Thread.h"
#include <filesystem>
#include <chrono>

static LogModule* gModule;

LogModule::LogModule(std::wstring_view ModuleName, size_t QueueSize)
	: ModuleName(ModuleName)
	, MessageQueue(QueueSize)
{
	check(gModule == nullptr);
	gModule = this;
}

LogModule::~LogModule()
{
	Shutdown();
}

void LogModule::RunTask()
{
	std::filesystem::path Directory = L"Saved/Logs";
	std::filesystem::path LogPath = std::format(L"{}.log", ModuleName);

	if (!std::filesystem::exists(Directory))
	{
		std::filesystem::create_directories(Directory);
	}
	else if (std::filesystem::exists(Directory / LogPath))
	{
		auto BackupTime = DateTime<>::Now().ToString();
		//auto BackupTime = std::chrono::steady_clock::now().time_since_epoch().count();
		std::filesystem::path BackupPath = StringUtils::ReplaceAll(std::format(L"{}_{}.log", ModuleName, BackupTime), L":", L"-");
		std::filesystem::rename(Directory / LogPath, Directory / BackupPath);
	}

	LogFile.open(Directory / LogPath, std::ios::trunc | std::ios::out);
	check(LogFile.is_open());

	bRunning = true;
	WorkerThread = Thread::NewThread<void>(L"[Log Module]", [&]()
	{
		Worker();
	});
}

void LogModule::Shutdown()
{
	if (bRunning)
	{
		gModule = nullptr;
		bRunning = false;

		WorkerThread.get();
		LogFile.close();

		MessageQueue.clear();
	}
}

void LogModule::EnqueueLogMessage(std::wstring_view Message)
{
	size_t MyIdx = QueueIndex++ % MessageQueue.size();
	MessageQueue[MyIdx] = std::wstring(Message);
	++SeekIndex;
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

	const size_t QueueSize = MessageQueue.size();
	size_t Seekpos = 0;

	while (bRunning)
	{
		std::this_thread::sleep_for(1s);
		size_t Readp = SeekIndex % QueueSize;

		for (size_t i = Seekpos; i != Readp; ++i)
		{
			if (i >= QueueSize)
			{
				i -= QueueSize;
			}

			LogFile << WCHAR_TO_ANSI(MessageQueue[i]) << std::endl;;
		}

		Seekpos = Readp;
		LogFile.flush();
	}
}