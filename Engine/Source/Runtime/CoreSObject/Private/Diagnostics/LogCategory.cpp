// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include <Windows.h>
#include <chrono>
#include <fstream>
#include "Misc/DateTime.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogModule.h"
#include "Threading/Thread.h"

LogCategory::LogCategory(std::wstring_view CategoryName)
	: CategoryName(CategoryName)
{
}

std::wstring_view LogCategory::GetName() const
{
	return CategoryName;
}

void LogCategory::OnLog(ELogVerbosity Verbosity, std::wstring_view Message)
{
	using namespace std;
	using namespace std::chrono;

	wstring Composed = format(L"{}: {}: {}", CategoryName, Verbosity.ToString(), Message);
	wstring DetailComposed = format(L"{}: {}: {}", DateTime::Now().ToString<libty::DateTimeFormat::File>(), Thread::GetCurrentThread()->GetFriendlyName(), Composed);

	// Log to Visual Studio Output Console.
	OutputDebugStringW((DetailComposed + L"\n").c_str());

	if (LogModule* Module = LogModule::Get(); Module && Module->IsRunning())
	{
		Module->EnqueueLogMessage(DetailComposed);
	}
}