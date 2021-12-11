// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Diagnostics/LogModule.h"
#include <filesystem>
#include <chrono>

GENERATE_BODY(SLogModule);

static SLogModule* gModule;

SLogModule::SLogModule(std::wstring_view ModuleName, size_t QueueSize) : Super()
	, ModuleName(ModuleName)
	, MessageQueue(QueueSize)
{
	check(gModule == nullptr);
	gModule = this;
}

SLogModule::~SLogModule()
{
	Shutdown();
}

void SLogModule::RunTask()
{
	std::filesystem::path Directory = L"Saved/Logs";
	std::filesystem::path LogPath = std::format(L"{}.log", ModuleName);

	if (!std::filesystem::exists(Directory))
	{
		std::filesystem::create_directories(Directory);
	}
	else if (std::filesystem::exists(Directory / LogPath))
	{
		auto BackupTime = std::chrono::zoned_time(std::chrono::system_clock::now()).get_local_time();
		std::filesystem::path BackupPath = StringUtils::ReplaceAll(std::format(L"{}_{}.log", ModuleName, BackupTime), L":", L"-");
		std::filesystem::rename(Directory / LogPath, Directory / BackupPath);
	}

	LogFile.open(Directory / LogPath, std::ios::trunc | std::ios::out);
	check(LogFile.is_open());

	bRunning = true;
	WorkerThread = std::thread(&SLogModule::Worker, this);
}

void SLogModule::Shutdown()
{
	if (bRunning)
	{
		bRunning = false;
		WorkerThread.join();
		LogFile.close();
	}
}

void SLogModule::EnqueueLogMessage(std::wstring_view Message)
{
	size_t MyIdx = QueueIndex++ % MessageQueue.size();
	MessageQueue[MyIdx] = Message;
	++SeekIndex;
}

bool SLogModule::IsRunning()
{
	return bRunning;
}

SLogModule* SLogModule::Get()
{
	return gModule;
}

void SLogModule::Worker()
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