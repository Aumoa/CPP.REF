// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Object.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Debug.gen.h"

namespace Ayla
{
	ACLASS()
	class CORE_API Debug : public Object
	{
		GENERATED_BODY()

	public:
		static void Log(const LogCategory& category, LogVerbosity logLevel, String message);

		template<class... TArgs>
		static inline void LogFormat(const LogCategory& category, LogVerbosity logLevel, String format, TArgs&&... args)
		{
			Log(category, logLevel, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogVerbose(const LogCategory& category, String message)
		{
			Log(category, LogVerbosity::Verbose, message);
		}

		template<class... TArgs>
		static inline void LogVerboseFormat(const LogCategory& category, String format, TArgs&&... args)
		{
			LogFormat(category, LogVerbosity::Verbose, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogInfo(const LogCategory& category, String message)
		{
			Log(category, LogVerbosity::Info, message);
		}

		template<class... TArgs>
		static inline void LogInfoFormat(const LogCategory& category, String format, TArgs&&... args)
		{
			LogFormat(category, LogVerbosity::Info, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogWarning(const LogCategory& category, String message)
		{
			Log(category, LogVerbosity::Warning, message);
		}

		template<class... TArgs>
		static inline void LogWarningFormat(const LogCategory& category, String format, TArgs&&... args)
		{
			LogFormat(category, LogVerbosity::Warning, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogError(const LogCategory& category, String message)
		{
			Log(category, LogVerbosity::Error, message);
		}

		template<class... TArgs>
		static inline void LogErrorFormat(const LogCategory& category, String format, TArgs&&... args)
		{
			LogFormat(category, LogVerbosity::Error, String::Format(format, std::forward<TArgs>(args)...));
		}

		static inline void LogCritical(const LogCategory& category, String message)
		{
			Log(category, LogVerbosity::Critical, message);
		}

		template<class... TArgs>
		static inline void LogCriticalFormat(const LogCategory& category, String format, TArgs&&... args)
		{
			LogFormat(category, LogVerbosity::Critical, String::Format(format, std::forward<TArgs>(args)...));
		}
	};
}