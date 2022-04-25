// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "Diagnostics/LogModule.h"
#include "Diagnostics/LogEntry.h"
#include "Misc/DateTime.h"
#include "Misc/String.h"
#include "Threading/Thread.h"
#include <filesystem>
#include <iostream>

using namespace libty;

LogModule::LogModule(std::wstring_view moduleName)
	: SingletonSupports(this)
	, _name(moduleName)
{
}

LogModule::~LogModule()
{
}

Task<> LogModule::StartAsync(std::stop_token cancellationToken)
{
	using namespace ::std::filesystem;

	path directory = L"Saved/Logs";
	path logPath = directory / path(String::Format(L"{}.log", _name));

	if (exists(logPath))
	{
		path newPath = path(logPath)
			.replace_filename(String::Format(L"{}_{}.log", _name, DateTime::Now().ToString<DateTimeFormat::File>()));
		rename(logPath, newPath);
	}
	else if (!exists(directory))
	{
		create_directory(directory);
	}

	_logFile.open(logPath, std::ios::trunc | std::ios::out);
	if (!_logFile.is_open())
	{
		throw InvalidOperationException("Cannot open file.");
	}

	auto tsc = TaskCompletionSource<>::Create();
	_thread = Thread::CreateThread(L"[Log Module]", std::bind(&LogModule::Worker, this, &tsc, _stopSource.get_token()));

	co_await tsc.GetTask();
}

Task<> LogModule::StopAsync(std::stop_token cancellationToken)
{
	_stopSource.request_stop();
	_cv.NotifyOne();
	return _thread->JoinAsync();
}

void LogModule::EnqueueLogMessage(LogEntry&& entry)
{
	std::unique_lock lock(_mutex);
	_entries.emplace_back(std::move(entry).Generate());
	_cv.NotifyOne();
}

bool LogModule::IsRunning()
{
	return _stopSource.stop_requested();
}

void LogModule::Worker(TaskCompletionSource<>* init, std::stop_token cancellationToken)
{
	using namespace std::literals;
	using namespace std::chrono;

	steady_clock::time_point Curr = steady_clock::now();
	std::vector<LogEntry> entries;
	init->SetResult();

	while (!cancellationToken.stop_requested())
	{
		std::unique_lock lock(_mutex);
		_cv.Wait(lock, [this, &cancellationToken]() { return _entries.size() > 0 || cancellationToken.stop_requested(); });

		std::swap(entries, _entries);
		lock.unlock();

		// Write messages.
		for (auto& entry : entries)
		{
			_logFile << String::AsMultibyte(entry.ToString(true)) << std::endl;
			Logged.Broadcast(entry);
			if (entry.Category->GetArguments().bLogToConsole)
			{
				std::wcout << entry.ToString() << std::endl;
			}
		}

		entries.clear();
		_logFile.flush();
	}
}