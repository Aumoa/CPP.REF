// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Diagnostics/Debug.h"
#include "Platform/PlatformProcess.h"
#include "Console.h"
#include "Marshal/ManagedStringWrapper.h"

namespace Ayla
{
	void Debug::Log(String category, LogVerbosity logLevel, String message)
	{
		auto output = String::Format(TEXT("{}: {}: {}"), category, logLevel, message);
		PlatformProcess::OutputDebugString(String::Format(TEXT("{}\n"), output));
	}
}

extern "C"
{
	PLATFORM_SHARED_EXPORT void Ayla__Debug__Log__Injected(::Ayla::ManagedStringWrapper category, ::Ayla::LogVerbosity logLevel, ::Ayla::ManagedStringWrapper message)
	{
		::Ayla::Debug::Log(category.AsString(), logLevel, message.AsString());
	}

	PLATFORM_SHARED_EXPORT void Ayla__Debug__EnsureMessage__Injected(bool condition, ::Ayla::ManagedStringWrapper message)
	{
		ensureMsgf(condition, ::Ayla::String::Format(TEXT("Ensure failed: {}"), message.AsString()));
	}

	PLATFORM_SHARED_EXPORT void Ayla__Debug__AssertMessage__Injected(bool condition, ::Ayla::ManagedStringWrapper message)
	{
		checkf(condition, ::Ayla::String::Format(TEXT("Assertion failed: {}"), message.AsString()));
	}
}