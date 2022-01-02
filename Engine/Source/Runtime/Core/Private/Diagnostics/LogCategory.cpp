// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <chrono>
#include <fstream>
#include "Misc/DateTime.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"

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
	using enum ELogVerbosity;

	switch (verbosity)
	{
	case Fatal: return L"Fatal";
	case Error: return L"Error";
	case Warning: return L"Warning";
	case Info: return L"Info";
	case Verbose: return L"Verbose";
	default: return L"";
	}
}

void LogCategory::OnLog(ELogVerbosity Verbosity, std::wstring_view Message)
{
	using namespace std;
	using namespace std::chrono;

	wstring Composed = format(L"{}: {}: {}", CategoryName, VerbosityToString(Verbosity), Message);
	wstring DetailComposed = format(L"{}: {}: {}", DateTime<>::Now().ToString(), Thread::GetCurrentThread()->GetFriendlyName(), Composed);

	// Log to Visual Studio Output Console.
	OutputDebugStringW(DetailComposed.c_str());
	OutputDebugStringW(L"\n");

	if (LogModule* Module = LogModule::Get(); Module && Module->IsRunning())
	{
		Module->EnqueueLogMessage(DetailComposed);
	}
}