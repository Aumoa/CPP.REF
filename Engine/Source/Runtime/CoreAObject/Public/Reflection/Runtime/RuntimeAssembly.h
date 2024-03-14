// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Reflection/Assembly.h"

class ARuntimeType;

class COREAOBJECT_API ARuntimeAssembly : public AAssembly
{
	AYLA_DECLARE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, RuntimeAssembly);
	AYLA_DECLARE_CLASS_TYPEDEFS(Engine, CoreAObject, RuntimeAssembly);

private:
	const String namespace_;
	const String assemblyName;
	const String fullName;
	std::vector<AType*> types;

public:
	ARuntimeAssembly(String namespace_, String assemblyName);

	virtual String GetName() const override;
	virtual String GetNamespace() const override;
	virtual String GetFullName() const override;
	virtual std::span<AType* const> GetTypes() const override;

	void AddRuntimeType(ARuntimeType* type);
};

#define AYLA_DEFINE_ASSEMBLY_INFO(Namespace, AssemblyName) \
ARuntimeAssembly& __Ayla_Get_Runtime_Assembly_ ## Namespace ## _ ## AssemblyName() \
{ \
	static ARuntimeAssembly s_RuntimeAssembly(TEXT(#Namespace), TEXT(#AssemblyName)); \
	return s_RuntimeAssembly; \
}