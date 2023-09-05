// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "SampleClass.h"

ASampleClass::ASampleClass()
{
}

AType* ASampleClass::StaticClass()
{
	static AType* TypePtr = nullptr;
	if (TypePtr == nullptr)
	{
		GENERATE_INTRINSIC_CLASS_METADATA(SampleClass, EClassMetadata::None);
		NTypeGen::GenerateClassType(TypePtr, Metadata);
	}
	return TypePtr;
}