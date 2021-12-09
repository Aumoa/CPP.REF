// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <iostream>
#include <chrono>
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"

LogCategory::LogCategory(std::wstring_view categoryName)
	: _name(categoryName)
{
}

LogCategory::~LogCategory()
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

void LogCategory::OnLog(ELogVerbosity logVerbosity, std::wstring_view message)
{
	using namespace std;
	using namespace std::chrono;

	//if (!_file.has_value())
	//{
	//	_file.emplace(format(L"Saved\\Logs\\{}_{:%F}.log", L"Logs", zoned_time(system_clock::now())));
	//}

	//wfstream& stream = _file.value().OpenSharedStream(this, ios::app, true);
	wstring Composed = format(L"{}: {}: {}\n", _name, VerbosityToString(logVerbosity), message);
	wstring TimeComposed = format(L"{}: {}", zoned_time(system_clock::now()).get_local_time(), Composed);
	//if (stream.is_open())
	//{
	//	// Log to file.
	//	stream << TimeComposed;
	//	stream.flush();
	//}

	// Log to Visual Studio Output Console.
	OutputDebugStringW(TimeComposed.c_str());

	// Log to console if it is available.
	switch (logVerbosity)
	{
	case ELogVerbosity::Verbose:
	case ELogVerbosity::Warning:
	case ELogVerbosity::Info:
		std::wcout << Composed;
		break;
	case ELogVerbosity::Error:
	case ELogVerbosity::Fatal:
		std::wcerr << Composed;
		break;
	}
}