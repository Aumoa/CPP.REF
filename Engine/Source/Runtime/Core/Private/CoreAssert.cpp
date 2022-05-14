// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Misc/PlatformMacros.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogCategory.h"

using namespace libty;

void CoreAssert::Ensure(String exp, String msg, const std::source_location& location)
{
	using namespace ::Generated::LogCategories;

	if (!msg)
	{
		constexpr String s = TEXT("s");

		LogSystem::Log(location, LogAssert, ELogVerbosity::Error, TEXT("Ensure failed: !({})"), exp);
	}
	else
	{
		LogSystem::Log(location, LogAssert, ELogVerbosity::Error, TEXT("Ensure failed: !({})\n{}"), exp, msg);
	}
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