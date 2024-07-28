// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:StackOverflowException;

export import :Exception;

export class CORE_API StackOverflowException : public Exception
{
public:
	StackOverflowException()
		: Exception(TEXT("The requested operation caused a stack overflow."))
	{
	}
};