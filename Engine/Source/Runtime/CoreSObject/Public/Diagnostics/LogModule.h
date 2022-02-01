// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <atomic>
#include <fstream>
#include <future>
#include "Delegates/MulticastEvent.h"

class Thread;

class CORESOBJECT_API LogModule
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