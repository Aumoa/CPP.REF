// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogVerbosity.h"
#include "LogCore.h"

using namespace std;

using enum ELogVerbosity;

void CoreAssert::Assert(wstring_view msg, wstring_view file, int32 line)
{
	LogSystem::Log(LogAssert, Fatal, L"Assertion failed: {}({})\n{}", file, line, msg);
}

void CoreAssert::Ensure(wstring_view msg, wstring_view file, int32 line)
{
	LogSystem::Log(LogAssert, Error, L"Ensure failed: {}({})\n{}", file, line, msg);
}

void CoreAssert::DebugBreak()
{
	// Call built-in function.
	__debugbreak();
}