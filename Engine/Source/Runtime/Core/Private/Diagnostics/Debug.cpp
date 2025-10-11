// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/Debug.h"
#include "Platform/PlatformProcess.h"
#include "Console.h"
#include "Marshal/ManagedStringWrapper.h"

namespace Ayla
{
	void Debug::Log(String category, LogVerbosity logLevel, String message)
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
		auto output = String::Format(TEXT("{}: {}: {}"), category, LogVerbosityStr[(int32)logLevel], message);
		PlatformProcess::OutputDebugString(String::Format(TEXT("{}\n"), output));
		Console::WriteLine(output);
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__Debug__Log(::Ayla::ManagedStringWrapper category, ::Ayla::LogVerbosity logLevel, ::Ayla::ManagedStringWrapper message)
	{
		::Ayla::Debug::Log(category.AsString(), logLevel, message.AsString());
	}
}