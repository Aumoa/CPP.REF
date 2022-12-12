// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Logging/Log.h"
#include "Logging/LogCategory.h"
#include "Logging/LogEntry.h"
#include "Logging/ILoggingExtension.h"
#include "Threading/Spinlock.h"
#include "Threading/SpinlockConditionVariable.h"
#include "Misc/PlatformMisc.h"
#include "Console.h"
#include <future>

namespace
{
	static std::jthread sWorker;
	static bool sRunning;
	static Spinlock sLock;
	static SpinlockConditionVariable sCondVar;
	static std::vector<LogEntry> sEntries;
	static std::vector<std::function<void()>> sActions;
	static std::vector<std::unique_ptr<ILoggingExtension>> sExtensions;
}

void Log::Initialize()
{
	sRunning = true;
	sWorker = std::jthread(_worker);
}

void Log::Print(const LogCategory& logCategory, ELogLevel logLevel, const String& message)
{
	static int trap = (_trap_init(), 0);

	std::unique_lock lock(sLock);
	sEntries.emplace_back() =
	{
		.LogDate = DateTime::Now(),
		.Category = &logCategory,
		.LogThread = _get_threadid_or_name(),
		.Level = logLevel,
		.Message = message
	};
	sCondVar.NotifyOne();

	if (logLevel == ELogLevel::Fatal)
	{
		lock.unlock();
		FlushAll();
	}
}

void Log::Post(std::function<void()> work)
{
	static int trap = (_trap_init(), 0);

	std::unique_lock lock(sLock);
	sActions.emplace_back(work);
	sCondVar.NotifyOne();
}

void Log::FlushAll()
{
	static int trap = (_trap_init(), 0);

	std::promise<void> promise;
	std::future<void> future = promise.get_future();

	std::unique_lock lock(sLock);
	sActions.emplace_back([&]()
	{
		promise.set_value();
	});
	sCondVar.NotifyOne();

	lock.unlock();
	future.wait();
}

void Log::InstallExtension(std::unique_ptr<ILoggingExtension> extension)
{
	extension->Installed();

	auto lock = std::unique_lock(sLock);
	sExtensions.emplace_back(std::move(extension));
}

void Log::Cleanup()
{
	static int trap = (_trap_init(), 0);

	auto lock = std::unique_lock(sLock);
	sRunning = false;
	sCondVar.NotifyOne();
	lock.unlock();

	sWorker.join();

	for (auto& extension : sExtensions)
	{
		extension->Uninstalled();
	}

	sExtensions.clear();
}

String Log::_get_threadid_or_name() noexcept
{
	static thread_local String name = []()
	{
		String name = PlatformMisc::GetThreadName();
		if (name.length() == 0)
		{
			name = String::Format(TEXT("Thread {}"), PlatformMisc::GetCurrentThreadId());
		}
		return name;
	}();
	return name;
}

void Log::_trap_init()
{
	struct _cleanup
	{
		~_cleanup()
		{
			Log::Cleanup();
		}
	};

	static auto _s = (Initialize(), _cleanup());
}

void Log::_worker()
{
	std::vector<LogEntry> entries;
	std::vector<std::function<void()>> actions;

	static auto levelToString = [](ELogLevel level) -> String
	{
		switch (level)
		{
		case ELogLevel::Debug:
			return TEXT("dbug");
		case ELogLevel::Info:
			return TEXT("info");
		case ELogLevel::Warning:
			return TEXT("warn");
		case ELogLevel::Error:
			return TEXT("erro");
		default:
			return TEXT("ftal");
		}
	};

	static auto levelToColor = [](ELogLevel level) -> EConsoleColor
	{
		switch (level)
		{
		case ELogLevel::Debug:
			return EConsoleColor::White;
		case ELogLevel::Info:
			return EConsoleColor::DarkGreen;
		case ELogLevel::Warning:
			return EConsoleColor::DarkYellow;
		case ELogLevel::Error:
			return EConsoleColor::DarkRed;
		default:
			return EConsoleColor::Red;
		}
	};

	static auto previousColor = Console::GetForegroundColor();

	while (sRunning || !sEntries.empty() || !sActions.empty())
	{
		std::unique_lock lock(sLock);
		std::swap(entries, sEntries);
		std::swap(actions, sActions);
		lock.unlock();

		for (auto& entry : entries)
		{
			Console::SetForegroundColor(levelToColor(entry.Level));
			Console::Write(levelToString(entry.Level));
#if !SHIPPING
			PlatformMisc::OutputDebugString(levelToString(entry.Level));
#endif
			Console::SetForegroundColor(previousColor);

			String formatted;
			formatted += String::Format(TEXT(": {}[{}]\n"), entry.Category->GetCategoryName(), entry.LogThread);
			for (auto& line : entry.Message.Split('\n'))
			{
				formatted += String::Format(TEXT("      {}\n"), line.TrimStart());
			}
			for (auto& extension : sExtensions)
			{
				extension->TraceOne(entry);
			}

			Console::Write(formatted);
#if !SHIPPING
			PlatformMisc::OutputDebugString(formatted);
#endif
		}
		for (auto& extension : sExtensions)
		{
			extension->Flush();
		}

		for (auto& action : actions)
		{
			action();
		}

		entries.clear();
		actions.clear();
		lock.lock();
		sCondVar.Wait(lock, []()
		{
			return !sEntries.empty() || !sRunning || !sActions.empty();
		});
	}
}