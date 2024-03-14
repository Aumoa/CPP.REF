// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/ReflectionUtility.h"
#include "Object.h"

void ReflectionUtility::InjectTypeInfo(AObject* builtInInstance, AType* builtInType)
{
	if (builtInInstance->referencer != nullptr)
	{
		throw InvalidOperationException(TEXT("Do NOT use ReflectionUtility.InjectTypeInfo for non-built-in instance."));
	}

	builtInInstance->classType = builtInType;
}