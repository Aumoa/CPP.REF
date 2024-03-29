// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "System/ConstructorNotFoundException.h"

ConstructorNotFoundException::ConstructorNotFoundException(String className)
	: Exception(String::Format(TEXT("Constructor '{0}()' not found."), className))
{
}