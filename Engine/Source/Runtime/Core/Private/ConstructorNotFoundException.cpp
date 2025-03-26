// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "ConstructorNotFoundException.h"

namespace Ayla
{
	ConstructorNotFoundException::ConstructorNotFoundException(String className)
		: Exception(String::Format(TEXT("Constructor '{0}()' not found."), className))
	{
	}
}