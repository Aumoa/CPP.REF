// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "CoreAObject/ObjectInitializer.h"

AGameObject::AGameObject()
{
}

void AGameObject::Rename(AObject* InOuter, Name InName)
{
	Outer = InOuter;
	ObjectName = InName;
}

RefPtr<AGameObject> AGameObject::NewObject(AObject* InOuter, Name InName, TSubclassOf<AGameObject> InClass)
{
	auto Obj = AObject::NewObject<AGameObject>(InClass.GetType());
	if (Obj == nullptr)
	{
		return nullptr;
	}

	Obj->Outer = InOuter;
	Obj->ObjectName = InName;
	return Obj;
}