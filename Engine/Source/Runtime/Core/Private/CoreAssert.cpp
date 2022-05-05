// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Misc/PlatformMacros.h"
#include "Misc/StringView.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogCategory.h"

using namespace libty;

void CoreAssert::Ensure(StringView exp, StringView msg, const std::source_location& location)
{
	using namespace ::Generated::LogCategories;

	if (msg.empty())
	{
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