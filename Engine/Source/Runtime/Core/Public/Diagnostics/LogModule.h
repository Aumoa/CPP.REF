// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <atomic>
#include <fstream>
#include <future>

class CORE_API LogModule
{
	using This = LogModule;

private:
	std::wstring ModuleName;
	std::future<void> WorkerThread;
	std::atomic<bool> bRunning;
	std::ofstream LogFile;
	
	std::vector<std::wstring> MessageQueue;
	std::atomic<size_t> QueueIndex = 0;
	std::atomic<size_t> SeekIndex = 0;

public:
	LogModule(std::wstring_view ModuleName, size_t QueueSize = 1024);
	~LogModule();

	void RunTask();
	void Shutdown();
	void EnqueueLogMessage(std::wstring_view Message);
	bool IsRunning();

	static LogModule* Get();

public:
	DECLARE_MULTICAST_EVENT(LoggedEvent, std::wstring_view);
	LoggedEvent Logged;

private:
	void Worker();
};