// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Misc/PlatformMacros.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogCategory.h"

using namespace libty;

void CoreAssert::Ensure(std::string_view exp, std::wstring_view msg, const std::source_location& location)
{
	if (msg.empty())
	{
		LogSystem::Log(location, Generated::LogCategories::LogAssert, ELogVerbosity::Error, L"Ensure failed: !({})",
			String::AsUnicode(exp)
		);
	}
	else
	{
		LogSystem::Log(location, Generated::LogCategories::LogAssert, ELogVerbosity::Error, L"Ensure failed: !({})\n{}",
			String::AsUnicode(exp),
			msg
		);
	}
}

void CoreAssert::Ensure(std::string_view exp, std::string_view msg, const std::source_location& location)
{
	Ensure(exp, String::AsUnicode(msg), location);
}

void CoreAssert::DebugBreak()
{
#if !SHIPPING && PLATFORM_WINDOWS
	// Call built-in function.
	__debugbreak();
#endif
}