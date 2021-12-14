// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Console.h"

DECLARE_LOG_CATEGORY(, LogVSProjects);

template<class... TArgs>
void LogAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(L"LogVSProjects: Verbose: "s + Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(Format);
	SE_LOG(LogVSProjects, Verbose, Format);
}

template<class... TArgs>
void WarningAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(L"LogVSProjects: Warning: " + Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(Format);
	SE_LOG(LogVSProjects, Warning, Format);
}

template<class... TArgs>
void ErrorAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(L"LogVSProjects: Error: "s + Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(Format);
	SE_LOG(LogVSProjects, Error, Format);
}

template<class... TArgs>
void FatalAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(L"LogVSProjects: Fatal: "s + Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(Format);
	SE_LOG(LogVSProjects, Fatal, Format);
}