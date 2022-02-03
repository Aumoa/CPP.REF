// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "LogCore.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"

void CoreAssert::Assert(std::wstring_view msg, std::wstring_view file, int32 line)
{
	LogSystem::Log(LogAssert, ELogVerbosity::Fatal, L"Assertion failed: {}({})\n{}", file, line, msg);
}

void CoreAssert::Ensure(std::wstring_view msg, std::wstring_view file, int32 line)
{
	LogSystem::Log(LogAssert, ELogVerbosity::Error, L"Ensure failed: {}({})\n{}", file, line, msg);
}

void CoreAssert::DebugBreak()
{
	// Call built-in function.
	__debugbreak();
}