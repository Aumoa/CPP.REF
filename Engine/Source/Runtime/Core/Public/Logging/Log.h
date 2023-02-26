// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogLevel.h"
#include "CoreTypes/String.h"
#include <functional>

class LogCategory;
class ILoggingExtension;

class CORE_API Log
{
	friend class Core;
	Log() = delete;

private:
	static void Initialize();

public:
	static void Print(const LogCategory& logCategory, ELogLevel logLevel, const String& message);
	static void Post(std::function<void()> work);
	static void FlushAll();
	static void InstallExtension(std::unique_ptr<ILoggingExtension> extension);
	static void Cleanup();

public:
	template<class... TFormatArgs>
	static inline void Print(const LogCategory& logCategory, ELogLevel logLevel, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, logLevel, String::Format(format, std::forward<TFormatArgs>(args)...));
	}

	template<class... TFormatArgs>
	static inline void Debug(const LogCategory& logCategory, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, ELogLevel::Debug, String::Format(format, std::forward<TFormatArgs>(args)...));
	}

	template<class... TFormatArgs>
	static inline void Info(const LogCategory& logCategory, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, ELogLevel::Info, String::Format(format, std::forward<TFormatArgs>(args)...));
	}

	template<class... TFormatArgs>
	static inline void Warning(const LogCategory& logCategory, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, ELogLevel::Warning, String::Format(format, std::forward<TFormatArgs>(args)...));
	}

	template<class... TFormatArgs>
	static inline void Error(const LogCategory& logCategory, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, ELogLevel::Error, String::Format(format, std::forward<TFormatArgs>(args)...));
	}

	template<class... TFormatArgs>
	[[noreturn]]
	static inline void Fatal(const LogCategory& logCategory, const String& format, TFormatArgs&&... args)
	{
		Print(logCategory, ELogLevel::Fatal, String::Format(format, std::forward<TFormatArgs>(args)...));
		throw;
	}

private:
	static String _get_threadid_or_name() noexcept;
	static void _trap_init();
	static void _worker();
};