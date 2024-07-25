// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "System/LanguageSupportMacros.h"

export module Core:ConstructorNotFoundException;

export import :Exception;

class CORE_API ConstructorNotFoundException : public Exception
{
public:
	ConstructorNotFoundException(String className)
		: Exception(String::Format(TEXT("Constructor '{0}()' not found."), className))
	{
	}
};