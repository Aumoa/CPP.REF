// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:ArgumentOutOfRangeException;

export import :Exception;

export class CORE_API ArgumentOutOfRangeException : public Exception
{
public:
	ArgumentOutOfRangeException()
		: Exception(TEXT("Argument is out of range."))
	{
	}
};