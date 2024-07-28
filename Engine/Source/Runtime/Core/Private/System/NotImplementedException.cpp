// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:NotImplementedException;

export import :Exception;

export class NotImplementedException : public Exception
{
public:
	NotImplementedException()
		: Exception(TEXT("The function to call does not implemented."))
	{
	}
};