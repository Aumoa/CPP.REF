// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

std::thread Log::WorkerThread;
std::vector<TextWriter*> Log::OutputDevices;
Spinlock Log::Lock;
SpinlockConditionVariable Log::Trigger;
std::vector<LogEntry> Log::LogQueue;

void Log::InternalLog(ELogLevel InLogLevel, String InFormattedStr)
{
}

void Log::InitThread()
{
	static auto Initializer = []()
	{
		WorkerThread = std::thread(Log::Worker);
		return 0;
	}();
}

void Log::Worker()
{
}