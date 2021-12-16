// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Console.h"

DECLARE_LOG_CATEGORY(, LogSolutionInterface);

template<class... TArgs>
void LogAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(L"LogSolutionInterface: Verbose: "s + Format);
	SE_LOG(LogSolutionInterface, Verbose, Format);
}

template<class... TArgs>
void WarningAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(L"LogSolutionInterface: Warning: "s + Format);
	SE_LOG(LogSolutionInterface, Warning, Format);
}

template<class... TArgs>
void ErrorAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(L"LogSolutionInterface: Error: "s + Format);
	SE_LOG(LogSolutionInterface, Error, Format);
}

template<class... TArgs>
void FatalAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	std::wstring Format = std::format(Message, std::forward<TArgs>(InArgs)...);
	Console::WriteLine(L"LogSolutionInterface: Fatal: "s + Format);
	SE_LOG(LogSolutionInterface, Fatal, Format);
}