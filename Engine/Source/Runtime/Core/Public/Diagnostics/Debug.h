// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Diagnostics/LogVerbosity.h"

namespace Ayla
{
	class CORE_API Debug
	{
	public:
		static void Log(String category, LogVerbosity logLevel, String message);

		template<class... TArgs>
		static inline void Log(String category, LogVerbosity logLevel, String format, TArgs&&... args)
		{
			Log(category, logLevel, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogVerbose(String category, String message)
		{
			Log(category, LogVerbosity::Verbose, message);
		}

		template<class... TArgs>
		static inline void LogVerbose(String category, String format, TArgs&&... args)
		{
			Log(category, LogVerbosity::Verbose, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogInfo(String category, String message)
		{
			Log(category, LogVerbosity::Info, message);
		}

		template<class... TArgs>
		static inline void LogInfo(String category, String format, TArgs&&... args)
		{
			Log(category, LogVerbosity::Info, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogWarning(String category, String message)
		{
			Log(category, LogVerbosity::Warning, message);
		}

		template<class... TArgs>
		static inline void LogWarning(String category, String format, TArgs&&... args)
		{
			Log(category, LogVerbosity::Warning, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogError(String category, String message)
		{
			Log(category, LogVerbosity::Error, message);
		}

		template<class... TArgs>
		static inline void LogError(String category, String format, TArgs&&... args)
		{
			Log(category, LogVerbosity::Error, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogCritical(String category, String message)
		{
			Log(category, LogVerbosity::Critical, message);
		}

		template<class... TArgs>
		static inline void LogCritical(String category, String format, TArgs&&... args)
		{
			Log(category, LogVerbosity::Critical, String::Format(format, std::forward<TArgs>(args)...));
		}
	};
}