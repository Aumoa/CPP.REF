// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:NullReferenceException;

export import :Exception;

export class CORE_API NullReferenceException : public Exception
{
public:
	NullReferenceException()
		: Exception(TEXT("Object reference not set to an instance of an object."))
	{
	}
};