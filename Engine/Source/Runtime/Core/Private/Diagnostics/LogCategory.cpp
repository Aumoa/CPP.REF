// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"

std::optional<FileReference> LogCategory::_file;

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

	if (!_file.has_value())
	{
		_file = FileReference(format(L"Saved\\Logs\\{}_{:%F}.log", L"Logs", zoned_time(system_clock::now())));
	}

	wfstream& stream = _file.value().OpenSharedStream(this, ios::app, true);
	wstring composed = format(L"{}: {}: {}: {}\n", zoned_time(system_clock::now()).get_local_time(), _name, VerbosityToString(logVerbosity), message);
	if (stream.is_open())
	{
		// Log to file.
		stream << composed;
		stream.flush();
	}

	// Log to Visual Studio Output Console.
	OutputDebugStringW(composed.c_str());
}