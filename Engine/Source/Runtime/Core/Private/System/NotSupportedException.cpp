// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:NotSupportedException;

export import :Exception;

export class CORE_API NotSupportedException : public Exception
{
	NotSupportedException(std::exception_ptr InInnerException)
		: Exception(TEXT("The specified function or operation is not supported."), InInnerException)
	{
	}

	~NotSupportedException() noexcept
	{
	}
};