// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Reflection/Assembly.h"
#include "Reflection/Type.h"
#include "Reflection/TypeInfoMetadataGenerator.Impl.h"
#include "Reflection/FieldInfo.h"
#include "Reflection/FieldInfoMetadataGenerator.Impl.h"

GENERATE_BODY(SAssembly);

class SSampleClass : public SObject
{
	GENERATED_BODY(SSampleClass);

public:
	SPROPERTY(Value)
	int32 Value = 10;
};

SAssembly::SAssembly(std::string_view assemblyName)
	: _assemblyName(String::AsUnicode(assemblyName))
{
}