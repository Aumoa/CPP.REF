// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Runtime/RuntimeType.h"

ARuntimeType::ARuntimeType(AAssembly* assembly, String className)
	: assembly(assembly)
	, className(className)
{
}

AAssembly* ARuntimeType::GetAssembly() const
{
	return assembly;
}