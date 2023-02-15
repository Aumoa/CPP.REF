// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Logging/Log.h"
#include "Misc/PlatformMisc.h"
#include "Diagnostics/Stacktrace.h"

void CoreAssert::Assert(const String& exp, const String& msg)
{
	Log::Fatal(LogAssert, TEXT("{} (in expression: {})\n{}"), msg, exp, Stacktrace::Current());
#if !SHIPPING
	PlatformMisc::Debugbreak();
#endif
}