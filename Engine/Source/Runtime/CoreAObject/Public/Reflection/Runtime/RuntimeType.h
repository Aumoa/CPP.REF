// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/Type.h"
#include "Reflection/ReflectionUtility.h"

class ARuntimeAssembly;

class COREAOBJECT_API ARuntimeType : public AType
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, RuntimeType);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, RuntimeType);

private:
	AAssembly* const assembly;
	const String className;
	AType* const baseType;
	AObject* (*const constructor)();

public:
	ARuntimeType(ARuntimeAssembly* assembly, String className, AType* baseType, AObject*(*constructor)());

	virtual AAssembly* GetAssembly() const override;
	virtual String GetName() const override;
	virtual AType* GetBaseType() const override;

	AObject* CreateInstance();
};

#define AYLA_DEFINE_CLASS_INFO(Namespace, AssemblyName, ClassName) \
ARuntimeAssembly& __Ayla_Get_Runtime_Assembly_ ## Namespace ## _ ## AssemblyName(); \
ARuntimeType __Ayla_RuntimeType_ ## Namespace ## _ ## AssemblyName ## _ ## ClassName(&__Ayla_Get_Runtime_Assembly_ ## Namespace ## _ ## AssemblyName(), TEXT(#ClassName), A ## ClassName::Super::StaticClass(), ReflectionUtility::GetConstructorFunction<A ## ClassName>());