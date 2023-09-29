// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Type.h"
#include "CodeGen/TypeGen.h"
#include "CoreAObject/ObjectInitializer.h"

AType::AType() : Super(NObjectInitializer::Get().MarkInit(StaticClass()))
{
}

AType::AType(std::defer_lock_t) : Super(NObjectInitializer::Get().MarkInit(this))
{
}

AType::~AType() noexcept
{
}

String AType::GetClassName() noexcept
{
	return ClassName;
}

AType* AType::StaticClass()
{
	static AType* TypePtr = nullptr;
	if (TypePtr == nullptr)
	{
		GENERATE_INTRINSIC_CLASS_METADATA(Type, EClassMetadata::Abstract);
		NTypeGen::GenerateClassType(TypePtr, Metadata);
	}
	return TypePtr;
}

REGISTER_INTRINSIC_CLASS(AType, TEXT("/Script/CoreAObject"), AType::Super::StaticClass());