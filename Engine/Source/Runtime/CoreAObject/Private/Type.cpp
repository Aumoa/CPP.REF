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

bool AType::IsDerivedFrom(AType* InType) const noexcept
{
	const AType* Current = this;
	while (Current->SuperClass)
	{
		if (Current->SuperClass == InType)
		{
			return true;
		}
		Current = Current->SuperClass;
	}
	return false;
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

extern "C"
{
	COREAOBJECT_API void* NativeCall_CoreAObject_Type_Construct(void* __this)
	{
		return ((AType*)__this)->Construct();
	}

	COREAOBJECT_API void* NativeCall_CoreAObject_Type_StaticClass()
	{
		return (AObject*)AType::StaticClass();
	}
}