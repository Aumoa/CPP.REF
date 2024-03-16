// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/Runtime/RuntimeAssembly.h"
#include "Reflection/Runtime/RuntimeType.h"

#define AYLA_DEFINE_DESTROY_FUNCTION(Namespace, AssemblyName, ClassName) \
extern "C" PLATFORM_SHARED_EXPORT void __Ayla_Destroy_ ## Namespace ## _ ## AssemblyName ## _ ## ClassName(ObjectReference* reference) \
{ \
	if (reference == nullptr) \
	{ \
		return; \
	} \
	\
	ObjectReference::Destroy(*reference); \
}

#define AYLA_DEFINE_CONSTRUCTOR_FUNCTION(Namespace, AssemblyName, ClassName) \
extern "C" PLATFORM_SHARED_EXPORT A ## ClassName* __Ayla_ConstructorInfo_ ## Namespace ## _ ## AssemblyName ## _ ## ClassName() \
{ \
	return (A ## ClassName*)AObject::NewObject(A ## ClassName::StaticClass()); \
}

#define GENERATED_BODY(...)
#define ACLASS(...)
