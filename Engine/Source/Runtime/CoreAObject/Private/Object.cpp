// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Object.h"
#include "Type.h"
#include "CodeGen/TypeGen.h"
#include "CoreAObject/ObjectInitializer.h"

AObject::AObject() : AObject(NObjectInitializer::Get())
{
}

AObject::AObject(NObjectInitializer& Initializer)
	: ClassType(Initializer.ConsumeConstructType())
	, Refs(new Referencer())
{
}

AObject::~AObject() noexcept
{
}

String AObject::ToString()
{
	return GetType()->ClassName;
}

AType* AObject::GetType()
{
	return ClassType;
}

AType* AObject::StaticClass()
{
	static AType* TypePtr = nullptr;
	if (TypePtr == nullptr)
	{
		GENERATE_INTRINSIC_CLASS_METADATA(Object, EClassMetadata::Abstract);
		NTypeGen::GenerateClassType(TypePtr, Metadata);
	}
	return TypePtr;
}

RefPtr<AObject> AObject::NewObject(AType* InClassType)
{
	check(InClassType);
	check((InClassType->ClassMeta & EClassMetadata::Abstract) == 0);
	NObjectInitializer::Get().MarkInit(InClassType);
	return RefPtr<AObject>(InClassType->Constructor());
}

REGISTER_INTRINSIC_CLASS(AObject, TEXT("/Script/CoreAObject"), nullptr);