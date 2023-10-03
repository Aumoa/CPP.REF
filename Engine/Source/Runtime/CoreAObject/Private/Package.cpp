// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Package.h"

APackage::APackage()
{
}

APackage::~APackage() noexcept
{
}

AType* APackage::StaticClass()
{
	static AType* TypePtr = nullptr;
	if (TypePtr == nullptr)
	{
		GENERATE_INTRINSIC_CLASS_METADATA(Package, EClassMetadata::Abstract);
		NTypeGen::GenerateClassType(TypePtr, Metadata);
	}
	return TypePtr;
}

REGISTER_INTRINSIC_CLASS(APackage, TEXT("/Script/CoreAObject"), APackage::Super::StaticClass());