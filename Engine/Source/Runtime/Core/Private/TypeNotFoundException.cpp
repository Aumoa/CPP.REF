// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "TypeNotFoundException.h"

namespace Ayla
{
	TypeNotFoundException::TypeNotFoundException(String className) : Exception(String::Format(TEXT("Type {} is not registered."), className))
	{
	}
}