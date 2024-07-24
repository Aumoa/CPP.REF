// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:OperationCanceledException;

export import :Exception;

class CORE_API OperationCanceledException : public Exception
{
public:
	OperationCanceledException(std::exception_ptr InInnerException = nullptr, std::source_location Src = std::source_location::current())
		: Exception(TEXT("Operation was aborted."), InInnerException, Src)
	{
	}
};