// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Logging/Log.h"
#include "Stacktrace/Stacktrace.h"

void CoreAssert::Assert(const String& exp, const String& msg)
{
	Log::Fatal(LogAssert, TEXT("{} (in expression: {})\n{}"), msg, exp, Stacktrace::CaptureCurrent().Trace());
	DebugBreak();
}

void CoreAssert::DebugBreak()
{
#if !SHIPPING
#if PLATFORM_WINDOWS

	// Call built-in function.
	__debugbreak();

#endif
#endif
}