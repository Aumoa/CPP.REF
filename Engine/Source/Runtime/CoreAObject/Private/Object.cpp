// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Object.h"
#include "ObjectReference.h"
#include "Reflection/ObjectMacros.h"

AYLA_DEFINE_CONSTRUCTOR_FUNCTION(Engine, CoreAObject, Object);
AYLA_DEFINE_DESTROY_FUNCTION(Engine, CoreAObject, Object);
AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, Object);

struct AObject::ObjectInitializer
{
	AType* classType = nullptr;

	static ObjectInitializer& Get()
	{
		static thread_local ObjectInitializer s_Current;
		return s_Current;
	}
};

AObject::AObject()
	: classType(ObjectInitializer::Get().classType)
	, referencer(new ObjectReference(this))
{
}

AObject::~AObject() noexcept
{
}

AObject* AObject::NewObject(AType* classType)
{
	ObjectInitializer::Get().classType = classType;
	return new AObject();
}

void AObject::Destroy(AObject* instance)
{
	if (instance == nullptr)
	{
		return;
	}

	ObjectReference::Destroy(*instance->referencer);
}

AType* AObject::StaticClass()
{
	return &__Ayla_RuntimeType_Engine_CoreAObject_Object;
}