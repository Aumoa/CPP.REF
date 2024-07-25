// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:ArgumentException;

export import :Exception;

export class CORE_API ArgumentException : public Exception
{
public:
	ArgumentException(String InArgumentName)
		: Exception(String::Format(TEXT("Invalid argument {} detected."), InArgumentName))
	{
	}
};