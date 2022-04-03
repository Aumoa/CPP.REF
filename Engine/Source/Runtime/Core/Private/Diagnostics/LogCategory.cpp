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

LogCategory::LogCategory(std::wstring_view CategoryName, Arguments&& InArgs)
	: CategoryName(CategoryName)
	, Args(std::move(InArgs))
{
}

std::wstring_view LogCategory::GetName() const
{
	return CategoryName;
}

std::wstring_view LogCategory::VerbosityToString(ELogVerbosity verbosity)
{
	switch (verbosity)
	{
	case ELogVerbosity::Error: return L"Error";
	case ELogVerbosity::Warning: return L"Warning";
	case ELogVerbosity::Info: return L"Info";
	case ELogVerbosity::Verbose: return L"Verbose";
	default: return L"";
	}
}

auto LogCategory::GetArguments() -> Arguments
{
	return Args;
}

void LogCategory::OnLog(ELogVerbosity Verbosity, std::wstring_view Message, const std::source_location& Src)
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