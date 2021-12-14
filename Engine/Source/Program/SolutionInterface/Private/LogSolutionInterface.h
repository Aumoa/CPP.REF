// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Console.h"

DECLARE_LOG_CATEGORY(, LogSolutionInterface);

template<class... TArgs>
void LogAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	Console::WriteLine(L"LogSolutionInterface: Verbose: " + Message, std::forward<TArgs>(InArgs)...);
	SE_LOG(LogSolutionInterface, Verbose, Message, std::forward<TArgs>(InArgs)...);
}

template<class... TArgs>
void WarningAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	Console::WriteLine(L"LogSolutionInterface: Warning: " + Message, std::forward<TArgs>(InArgs)...);
	SE_LOG(LogSolutionInterface, Warning, Message, std::forward<TArgs>(InArgs)...);
}

template<class... TArgs>
void ErrorAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	Console::WriteLine(L"LogSolutionInterface: Error: " + Message, std::forward<TArgs>(InArgs)...);
	SE_LOG(LogSolutionInterface, Error, Message, std::forward<TArgs>(InArgs)...);
}

template<class... TArgs>
void FatalAndDisplay(const std::wstring& Message, TArgs&&... InArgs)
{
	using namespace std::literals;
	Console::WriteLine(L"LogSolutionInterface: Fatal: " + Message, std::forward<TArgs>(InArgs)...);
	SE_LOG(LogSolutionInterface, Fatal, Message, std::forward<TArgs>(InArgs)...);
}