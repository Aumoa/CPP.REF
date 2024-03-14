// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Object.h"
#include "ObjectReference.h"
#include "Reflection/ObjectMacros.h"
#include "Reflection/Activator.h"

AYLA_DEFINE_CONSTRUCTOR_FUNCTION(Engine, CoreAObject, Object);
AYLA_DEFINE_DESTROY_FUNCTION(Engine, CoreAObject, Object);
AYLA_DEFINE_CLASS_INFO(Engine, CoreAObject, Object);
AYLA_DEFINE_STATIC_CLASS_FUNCTION(Engine, CoreAObject, Object);

struct AObject::ObjectInitializer
{
	AType* classType = nullptr;

	void Clear()
	{
		classType = nullptr;
	}

	static ObjectInitializer& Get()
	{
		static thread_local ObjectInitializer s_Current;
		return s_Current;
	}
};

AObject::AObject(std::in_place_t builtIn)
{
}

AObject::AObject()
	: classType(ObjectInitializer::Get().classType)
	, referencer(new ObjectReference(this))
{
	if (classType == nullptr)
	{
		throw InvalidOperationException(TEXT("ObjectInitializer is not ready. Do NOT use Activator.CreateInstance instead AObject.NewObject."));
	}
}

AObject::~AObject() noexcept
{
}

AType* AObject::GetType()
{
	return classType;
}

AObject* AObject::NewObject(AType* classType)
{
	return TryFinally(
		[&]()
		{
			ObjectInitializer::Get().classType = classType;
			return Activator::CreateInstance(classType);
		},
		[]()
		{
			ObjectInitializer::Get().Clear();
		}
	).Execute();
}

void AObject::Destroy(AObject* instance)
{
	if (instance == nullptr)
	{
		return;
	}

	if (instance->referencer == nullptr)
	{
		throw InvalidOperationException(TEXT("Cannot destroy built-in system instances."));
	}

	ObjectReference::Destroy(*instance->referencer);
}