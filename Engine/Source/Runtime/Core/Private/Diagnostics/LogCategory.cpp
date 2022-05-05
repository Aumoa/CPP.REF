// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/DateTime.h"
#include "Misc/PlatformMacros.h"
#include "Misc/String.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogModule.h"
#include "Diagnostics/LogEntry.h"
#include "Threading/Thread.h"
#include <chrono>
#include <fstream>
#include <filesystem>

using namespace libty;

LogCategory::LogCategory(StringView CategoryName, Arguments&& InArgs)
	: CategoryName(CategoryName)
	, Args(std::move(InArgs))
{
}

StringView LogCategory::GetName() const
{
	return CategoryName;
}

StringView LogCategory::VerbosityToString(ELogVerbosity verbosity)
{
	switch (verbosity)
	{
	case ELogVerbosity::Error: return TEXT("Error");
	case ELogVerbosity::Warning: return TEXT("Warning");
	case ELogVerbosity::Info: return TEXT("Info");
	case ELogVerbosity::Verbose: return TEXT("Verbose");
	default: return TEXT("");
	}
}

auto LogCategory::GetArguments() -> Arguments
{
	return Args;
}

void LogCategory::OnLog(ELogVerbosity Verbosity, StringView Message, const std::source_location& Src)
{
	auto thr = Thread::GetCurrentThread();
	LogEntry entry;
	entry.ThreadId = thr->GetThreadId();
	entry.ThreadName = thr->GetFriendlyName();
	entry.LoggedTime = DateTime::Now();
	entry.Category = this;
	entry.Verbosity = Verbosity;
	entry.Message = Message;
	entry.Source = Src;

	if (LogModule* Module = LogModule::Get(); Module && Module->IsRunning())
	{
		Module->EnqueueLogMessage(std::move(entry));
	}
}