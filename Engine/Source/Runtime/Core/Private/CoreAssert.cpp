// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogCategory.h"

void CoreAssert::Assert(std::wstring_view msg, const std::source_location& location)
{
	LogSystem::Log(location, LogAssert, ELogVerbosity::Fatal, L"Assertion failed: {}", msg);
}

void CoreAssert::Ensure(std::wstring_view msg, const std::source_location& location)
{
	LogSystem::Log(location, LogAssert, ELogVerbosity::Error, L"Ensure failed: {}", msg);
}

void CoreAssert::DebugBreak()
{
	// Call built-in function.
	__debugbreak();
}