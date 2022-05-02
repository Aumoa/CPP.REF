// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAssert.h"
#include "Diagnostics/LogModule.h"
#include "Diagnostics/LogEntry.h"
#include "Misc/DateTime.h"
#include "Misc/String.h"
#include "Threading/Thread.h"
#include "IO/Directory.h"
#include "IO/File.h"
#include <filesystem>
#include <iostream>

using namespace libty;

LogModule* LogModule::sInstance;

LogModule::LogModule(std::wstring_view moduleName)
	: _name(moduleName)
{
	check(sInstance == nullptr);
	sInstance = this;
}

LogModule::~LogModule()
{
	sInstance = nullptr;
}

Task<> LogModule::StartAsync(std::stop_token cancellationToken)
{
	using namespace ::std::filesystem;

	path directory = L"Saved/Logs";
	path logPath = directory / path(String::Format(L"{}.log", _name));

	if (File::Exists(logPath))
	{
		path newPath = path(logPath)
			.replace_filename(String::Format(L"{}_{}.log", _name, DateTime::Now().ToString<DateTimeFormat::File>()));
		rename(logPath, newPath);
	}
	else if (!Directory::Exists(directory))
	{
		Directory::TryCreateDirectory(directory);
	}

	_logFile.open(logPath, std::ios::trunc | std::ios::out);
	if (!_logFile.is_open())
	{
		throw InvalidOperationException("Cannot open file.");
	}

	_thread = Thread::CreateThread(L"[Log Module]", std::bind(&LogModule::Worker, this, _stopSource.get_token()));
	return Task<>::CompletedTask();
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

void LogModule::EnqueueLogAction(std::function<void()> action)
{
	std::unique_lock lock(_mutex);
	_entries.emplace_back(std::move(action));
	_cv.NotifyOne();
}

bool LogModule::IsRunning()
{
	return !_stopSource.stop_requested();
}

void LogModule::Worker(std::stop_token cancellationToken)
{
	using namespace std::literals;
	using namespace std::chrono;

	std::vector<Variant_t> entries;

	while (!cancellationToken.stop_requested())
	{
		std::unique_lock lock(_mutex);
		_cv.Wait(lock, [this, &cancellationToken]() { return _entries.size() > 0 || cancellationToken.stop_requested(); });

		std::swap(entries, _entries);
		lock.unlock();

		for (auto& entry : entries)
		{
			switch (entry.index())
			{
			case 0:  // LogEntry
				Logged.Broadcast(std::get<0>(entry));
				break;
			case 1:
				std::get<1>(entry)();
				break;
			}
		}

		entries.clear();
		_logFile.flush();
	}
}