// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogModule.h"

LogCategory::LogCategory(std::wstring_view CategoryName)
	: CategoryName(CategoryName)
{
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
	wstring TimeComposed = format(L"{}: {}", zoned_time(system_clock::now()).get_local_time(), Composed);

	// Log to Visual Studio Output Console.
	OutputDebugStringW(TimeComposed.c_str());
	OutputDebugStringW(L"\n");

	// Log to console if it is available.
	switch (Verbosity)
	{
	case ELogVerbosity::Verbose:
	case ELogVerbosity::Warning:
	case ELogVerbosity::Info:
		wcout << Composed << endl;
		break;
	case ELogVerbosity::Error:
	case ELogVerbosity::Fatal:
		wcerr << Composed << endl;
		break;
	}

	if (SLogModule* Module = SLogModule::Get(); Module && Module->IsRunning())
	{
		Module->EnqueueLogMessage(TimeComposed);
	}
}