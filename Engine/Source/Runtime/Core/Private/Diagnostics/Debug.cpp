// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/Debug.h"
#include "Platform/PlatformProcess.h"

namespace Ayla
{
	void Debug::Log(const LogCategory& category, LogVerbosity logLevel, String message)
	{
		static String LogVerbosityStr[] =
		{
			TEXT("Verbose"),
			TEXT("Info"),
			TEXT("Warning"),
			TEXT("Error"),
			TEXT("Critical")
		};

		// TODO:
		PlatformProcess::OutputDebugString(String::Format(TEXT("{}: {}: {}\n"), category.GetName(), LogVerbosityStr[(int)logLevel], message));
	}
}