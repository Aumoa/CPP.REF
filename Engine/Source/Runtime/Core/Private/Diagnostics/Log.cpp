// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

std::vector<TextWriter*> Log::OutputDevices;
Spinlock Log::Lock;
SpinlockConditionVariable Log::Trigger;
std::vector<LogEntry> Log::LogQueue;

SpinlockConditionVariable FlushConditionVar;

void Log::FlushAll()
{
	while (true)
	{
		std::unique_lock ScopedLock(Lock);
		if (LogQueue.empty())
		{
			break;
		}

		FlushConditionVar.Wait(ScopedLock, []() { return LogQueue.empty(); });
	}
}

void Log::InternalLog(ELogLevel InLogLevel, String InFormattedStr)
{
	InitThread();

	LogEntry Entry;
	Entry.LogDate = DateTime::Now();
	Entry.LogThread = PlatformMisc::GetThreadName();
	Entry.Level = InLogLevel;
	Entry.Message = InFormattedStr;

	std::unique_lock ScopedLock(Lock);
	LogQueue.emplace_back(std::move(Entry));
	Trigger.NotifyOne();
}

void Log::InitThread()
{
	static auto Initializer = []()
	{
		std::thread(Log::Worker).detach();
		return 0;
	}();
}

void Log::Worker()
{
	PlatformMisc::SetThreadName(TEXT("Log Thread"));

	std::vector<LogEntry> SwapQueue;

	while (true)
	{
		SwapQueue.clear();

		{
			std::unique_lock ScopedLock(Lock);
			std::swap(SwapQueue, LogQueue);
		}

		if (SwapQueue.size() == 0)
		{
			std::unique_lock ScopedLock(Lock);
			Trigger.Wait(ScopedLock, []() { return LogQueue.size() > 0; });
			std::swap(SwapQueue, LogQueue);
		}

		EConsoleColor ForegroundColor, BackgroundColor;
		Console::GetColors(&ForegroundColor, &BackgroundColor);

		auto ScopedTextColor = [&](String InStr, EConsoleColor InForegroundColor, EConsoleColor InBackgroundColor)
		{
			Console::SetColors(InForegroundColor, InBackgroundColor);
			Console::Write(InStr);
			Console::SetColors(ForegroundColor, BackgroundColor);
		};

		for (size_t i = 0; i < SwapQueue.size(); ++i)
		{
			LogEntry& Current = SwapQueue[i];

			switch (Current.Level)
			{
			case ELogLevel::Trace:
				ScopedTextColor(TEXT("trce"), EConsoleColor::Gray, BackgroundColor);
				break;
			case ELogLevel::Debug:
				ScopedTextColor(TEXT("dbug"), EConsoleColor::Gray, BackgroundColor);
				break;
			case ELogLevel::Information:
				ScopedTextColor(TEXT("info"), EConsoleColor::DarkGreen, BackgroundColor);
				break;
			case ELogLevel::Warning:
				ScopedTextColor(TEXT("warn"), EConsoleColor::Yellow, BackgroundColor);
				break;
			case ELogLevel::Error:
				ScopedTextColor(TEXT("fail"), EConsoleColor::Black, EConsoleColor::DarkRed);
				break;
			case ELogLevel::Critical:
				ScopedTextColor(TEXT("crit"), EConsoleColor::White, EConsoleColor::DarkRed);
				break;
			}

			std::vector<String> Lines = Current.Message.Split('\n', EStringSplitOptions::TrimEntries);
			Console::WriteLine(TEXT(": {}"), Lines.front());
			for (size_t j = 1; j < Lines.size(); ++j)
			{
				// padding level string. ex: "info: "
				Console::WriteLine(TEXT("      {}"), Lines[j]);
			}
		}

		{
			std::unique_lock ScopedLock(Lock);
			FlushConditionVar.NotifyAll();
		}
	}
}