// Copyright 2020 Aumoa.lib. All right reserved.

#include "Logging/Logger.h"

#include "WindowsMinimal.h"
#include "Logging/LogVerbosity.h"
#include "Logging/LogCategoryBase.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::Logging;

void Logger::Log(LogCategoryBase& category, ELogVerbosity inVerbosity, TRefPtr<String> logMessage)
{
	TRefPtr<String> categoryName = category.CategoryName;
	const wchar_t* verbosityName = ToString(category.GetAmendedVerbosity(inVerbosity));

	OutputDebugStringW(String::Format(L"{0}: {1}: {2}\n", categoryName, verbosityName, logMessage)->C_Str);
}

const wchar_t* Logger::ToString(ELogVerbosity inVerbosity)
{
	switch (inVerbosity)
	{
	case ELogVerbosity::Fatal:
		return L"Fatal";
	case ELogVerbosity::Error:
		return L"Error";
	case ELogVerbosity::Warning:
		return L"Warning";
	case ELogVerbosity::Display:
		return L"Display";
	case ELogVerbosity::Log:
		return L"Log";
	case ELogVerbosity::Verbose:
		return L"Verbose";
	case ELogVerbosity::VeryVerbose:
		return L"VeryVerbose";
	default:
		throw InvalidArgumentException();
	}
}