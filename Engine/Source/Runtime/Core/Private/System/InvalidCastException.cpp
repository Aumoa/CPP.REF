// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:InvalidCastException;

export import :Exception;

export class CORE_API InvalidCastException : public Exception
{
public:
	InvalidCastException(String InMessage, std::exception_ptr InInnerException)
		: Exception(InMessage, InInnerException)
	{
	}
};