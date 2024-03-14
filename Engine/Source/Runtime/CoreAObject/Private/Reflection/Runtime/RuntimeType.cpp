// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Runtime/RuntimeType.h"
#include "Reflection/Runtime/RuntimeAssembly.h"

AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, RuntimeType);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, RuntimeType);

ARuntimeType::ARuntimeType(ARuntimeAssembly* assembly, String className, AType* baseType)
	: assembly(assembly)
	, className(className)
	, baseType(baseType)
{
	assembly->AddRuntimeType(this);
}

AAssembly* ARuntimeType::GetAssembly() const
{
	return assembly;
}

String ARuntimeType::GetName() const
{
	return className;
}

AType* ARuntimeType::GetBaseType() const
{
	return baseType;
}