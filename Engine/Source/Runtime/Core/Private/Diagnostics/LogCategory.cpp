// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/DateTime.h"
#include "Misc/PlatformMacros.h"
#include "Misc/String.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"
#include <chrono>
#include <fstream>

#if PLATFORM_WINDOWS
#include <Windows.h>
#endif

LogCategory::LogCategory(std::wstring_view CategoryName)
	: CategoryName(CategoryName)
{
}

std::wstring_view LogCategory::GetName() const
{
	return CategoryName;
}

std::wstring_view LogCategory::VerbosityToString(ELogVerbosity verbosity)
{
	switch (verbosity)
	{
	case ELogVerbosity::Fatal: return L"Fatal";
	case ELogVerbosity::Error: return L"Error";
	case ELogVerbosity::Warning: return L"Warning";
	case ELogVerbosity::Info: return L"Info";
	case ELogVerbosity::Verbose: return L"Verbose";
	default: return L"";
	}
}

void LogCategory::OnLog(ELogVerbosity Verbosity, std::wstring_view Message)
{
	using namespace std;
	using namespace std::chrono;

	wstring Composed = String::Format(L"{}: {}: {}", CategoryName, VerbosityToString(Verbosity), Message);
	wstring DetailComposed = String::Format(L"{}: {}: {}", DateTime::Now().ToString<libty::DateTimeFormat::Json>(), Thread::GetCurrentThread()->GetFriendlyName(), Composed);

#if PLATFORM_WINDOWS
	// Log to Visual Studio Output Console.
	OutputDebugStringW((DetailComposed + L"\n").c_str());
#endif

	if (LogModule* Module = LogModule::Get(); Module && Module->IsRunning())
	{
		Module->EnqueueLogMessage(DetailComposed);
	}
}