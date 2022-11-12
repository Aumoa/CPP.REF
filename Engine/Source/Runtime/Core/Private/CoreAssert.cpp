// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Logging/Log.h"
#include "Misc/PlatformMisc.h"
#include <stacktrace>

void CoreAssert::Assert(const String& exp, const String& msg)
{
	Log::Fatal(LogAssert, TEXT("{} (in expression: {})\n{}"), msg, exp, String::FromCodepage(std::to_string(std::stacktrace::current())));
#if !SHIPPING
	PlatformMisc::Debugbreak();
#endif
}