// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/Type.h"

class COREAOBJECT_API ARuntimeType : public AType
{
	AAssembly* const assembly;
	const String className;

public:
	ARuntimeType(AAssembly* assembly, String className);

	virtual AAssembly* GetAssembly() const override;
};

#define AYLA_DEFINE_CLASS_INFO(Namespace, AssemblyName, ClassName) \
ARuntimeAssembly& __Ayla_Get_Runtime_Assembly_ ## Namespace ## _ ## AssemblyName(); \
ARuntimeType __Ayla_RuntimeType_ ## Namespace ## _ ## AssemblyName ## _ ## ClassName(&__Ayla_Get_Runtime_Assembly_ ## Namespace ## _ ## AssemblyName(), TEXT(#ClassName));