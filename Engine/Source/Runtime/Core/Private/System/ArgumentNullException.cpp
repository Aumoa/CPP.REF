// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:ArgumentNullException;

export import :Exception;

export class CORE_API ArgumentNullException : public Exception
{
public:
	ArgumentNullException(String InArgumentName)
		: Exception(String::Format(TEXT("Argument {} cannot be nullptr."), InArgumentName))
	{
	}
};