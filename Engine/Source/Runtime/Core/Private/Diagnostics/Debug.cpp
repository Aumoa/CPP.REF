// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/Debug.h"
#include "Platform/PlatformProcess.h"
#include "Console.h"

namespace Ayla
{
	void Debug::Injected_Log(String category, int32 logLevel, String message)
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
		auto output = String::Format(TEXT("{}: {}: {}"), category, LogVerbosityStr[logLevel], message);
		PlatformProcess::OutputDebugString(String::Format(TEXT("{}\n"), output));
		Console::WriteLine(output);
	}
}