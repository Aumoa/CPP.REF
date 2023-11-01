// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "SubclassOf.h"
#include "GameObject.generated.h"

ACLASS()
class ENGINE_API AGameObject : public AObject
{
	GENERATED_BODY()

private:
	AObject* Outer = nullptr;
	Name ObjectName;

public:
	AGameObject();
	
	void Rename(AObject* InOuter, Name InName);
	inline AObject* GetOuter() const { return Outer; }
	inline Name GetName() const { return ObjectName; }

	static RefPtr<AGameObject> NewObject(AObject* InOuter, Name InName, TSubclassOf<AGameObject> InClass);

	template<std::derived_from<AGameObject> T>
	static RefPtr<T> NewObject(AObject* InOuter, Name InName, TSubclassOf<T> InClass = T::StaticClass())
	{
		return NewObject(InOuter, InName, (TSubclassOf<AGameObject>)InClass);
	}
};