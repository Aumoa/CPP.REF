// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Runtime/RuntimeType.h"
#include "Reflection/Runtime/RuntimeAssembly.h"

AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, RuntimeType);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, RuntimeType);

ARuntimeType::ARuntimeType(ARuntimeAssembly* assembly, String className, AType* baseType, AObject*(*constructor)())
	: assembly(assembly)
	, className(className)
	, baseType(baseType)
	, constructor(constructor)
{
	assembly->AddRuntimeType(this);
	ReflectionUtility::InjectTypeInfo(this, this);
	if (assembly->GetType() == nullptr)
	{
		ReflectionUtility::InjectTypeInfo(assembly, ARuntimeAssembly::StaticClass());
	}
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

AObject* ARuntimeType::CreateInstance()
{
	if (constructor == nullptr)
	{
		throw ConstructorNotFoundException(className);
	}

	return constructor();
}