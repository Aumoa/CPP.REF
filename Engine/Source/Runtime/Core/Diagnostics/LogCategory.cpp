// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import std.filesystem;
import SC.Runtime.Core;
import SC.Runtime.Core.Internal;

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

using enum ELogVerbosity;

optional<FileReference> LogCategory::_file;

LogCategory::LogCategory(wstring_view categoryName)
	: _name(categoryName)
{
}

LogCategory::~LogCategory()
{
}

wstring_view LogCategory::VerbosityToString(ELogVerbosity verbosity)
{
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

void LogCategory::OnLog(ELogVerbosity logVerbosity, wstring_view message)
{
	if (!_file.has_value())
	{
		_file = FileReference(format(L"Saved\\Logs\\{}_{:%F}.log", L"Logs", zoned_time(system_clock::now())));
	}

	wfstream& stream = _file.value().OpenSharedStream(this, ios::app, true);
	wstring composed = format(L"{}: Log{}: {}: {}\n", zoned_time(system_clock::now()).get_local_time(), _name, VerbosityToString(logVerbosity), message);
	if (stream.is_open())
	{
		stream << composed;
		stream.flush();
	}
	OutputDebugStringW(composed.c_str());
}