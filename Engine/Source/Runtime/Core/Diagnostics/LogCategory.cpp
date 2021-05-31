// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import std.filesystem;
import SC.Runtime.Core;

using namespace std;
using namespace std::chrono;
using namespace std::filesystem;

using enum ELogVerbosity;

LogCategory::LogCategory(wstring_view categoryName) : _name(categoryName)
{
}

zoned_time<system_clock::duration> LogCategory::GetZonedTime()
{
	system_clock::time_point systemNow = system_clock::now();
	return zoned_time(current_zone(), systemNow);
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

wstring LogCategory::GetTimedStringAppend(wstring_view name)
{
	return format(L"{}_{:%m-%d}", name, GetZonedTime());
}

void LogCategory::OnLog(ELogVerbosity logVerbosity, wstring_view message)
{
	if (_logfile && !_file.is_open())
	{
		wstring logFile = GetLogFilePath();
		_file.open(logFile, ios::app);
		if (!_file.is_open())
		{
			_logfile = false;
			OnLog(Error, format(L"Log to file is disabled. Cannot open logging file: {}", logFile));
		}
	}

	if (_logfile)
	{
		auto time = GetZonedTime();
		_file << time << L": Log" << _name << L": " << VerbosityToString(logVerbosity) << L": " << message << endl;
	}
}

path LogCategory::GetLogFilePath() const
{
	return L"Saved/Logs/" + GetTimedStringAppend(_name);
}