// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Reflection/Activator.h"
#include "Reflection/Runtime/RuntimeType.h"

AObject* Activator::CreateInstance(AType* classType)
{
	auto runtimeType = (ARuntimeType*)classType;
	return runtimeType->CreateInstance();
}