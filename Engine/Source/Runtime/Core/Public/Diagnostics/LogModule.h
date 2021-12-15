// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <atomic>
#include <fstream>
#include <future>

class CORE_API SLogModule : implements SObject
{
	GENERATED_BODY(SLogModule)

private:
	std::wstring ModuleName;
	std::future<void> WorkerThread;
	std::atomic<bool> bRunning;
	std::ofstream LogFile;
	
	std::vector<std::wstring> MessageQueue;
	std::atomic<size_t> QueueIndex = 0;
	std::atomic<size_t> SeekIndex = 0;

public:
	SLogModule(std::wstring_view ModuleName, size_t QueueSize = 1024);
	~SLogModule();

	void RunTask();
	void Shutdown();
	void EnqueueLogMessage(std::wstring_view Message);
	bool IsRunning();

	static SLogModule* Get();

private:
	void Worker();
};