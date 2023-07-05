// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.Diagnostics:Log;

export import Core.Std;
export import Core.System;
export import Core.Threading;
export import Core.IO;
export import :LogLevel;
export import :LogEntry;

export class CORE_API Log
{
public:
	template<class... TArgs>
	static void Write(ELogLevel InLogLevel, String InFormat, TArgs&&... InArgs)
	{
		InternalLog(InLogLevel, String::Format(InFormat, std::forward<TArgs>(InArgs)...));
	}

	template<class... TArgs>
	static void Trace(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Trace, InFormat, std::forward<TArgs>(InArgs)...);
	}

	template<class... TArgs>
	static void Debug(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Debug, InFormat, std::forward<TArgs>(InArgs)...);
	}

	template<class... TArgs>
	static void Information(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Information, InFormat, std::forward<TArgs>(InArgs)...);
	}

	template<class... TArgs>
	static void Warning(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Warning, InFormat, std::forward<TArgs>(InArgs)...);
	}

	template<class... TArgs>
	static void Error(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Error, InFormat, std::forward<TArgs>(InArgs)...);
	}

	template<class... TArgs>
	static void Critical(String InFormat, TArgs&&... InArgs)
	{
		Write(ELogLevel::Critical, InFormat, std::forward<TArgs>(InArgs)...);
	}

private:
	static void InternalLog(ELogLevel InLogLevel, String InFormattedStr);

public:
	static void AddOutputDevice(TextWriter& LogOutputDevice);
	static std::span<const TextWriter*> GetOutputDevices();
	static void ClearOutputDevice();

private:
	static std::thread WorkerThread;
	static std::vector<TextWriter*> OutputDevices;
	static Spinlock Lock;
	static SpinlockConditionVariable Trigger;
	static std::vector<LogEntry> LogQueue;

private:
	static void InitThread();
	static void Worker();
};