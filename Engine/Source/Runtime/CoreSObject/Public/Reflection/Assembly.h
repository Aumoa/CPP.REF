// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "PrimitiveTypes.h"

class CORESOBJECT_API SAssembly : implements SObject
{
	GENERATED_BODY(SAssembly);

private:
	std::wstring _assemblyName;

public:
	SAssembly(std::string_view assemblyName);
};

#define DEFINE_ASSEMBLY_INFO(...) \
static SAssembly AssemblyInfo(SE_ASSEMBLY_NAME);
