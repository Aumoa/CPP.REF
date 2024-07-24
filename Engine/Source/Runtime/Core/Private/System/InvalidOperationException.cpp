// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:InvalidOperationException;

export import :Exception;

export class CORE_API InvalidOperationException : public Exception
{
public:
	InvalidOperationException(const String& InMessage, std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(InMessage, InInnerException, Src)
	{
	}

	InvalidOperationException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Invalid operation detected."), InInnerException, Src)
	{
	}
};