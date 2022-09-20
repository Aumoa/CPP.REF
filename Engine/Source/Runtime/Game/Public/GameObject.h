// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.gen.h"

SCLASS()
class GAME_API GameObject : virtual public Object
{
	GENERATED_BODY()

private:
	SPROPERTY()
	String Name;
	SPROPERTY()
	Object* Outer = nullptr;

public:
	GameObject();

	void Rename(Object* InOuter, const String& InName);
	String GetName() noexcept { return Name; }
	Object* GetOuter() noexcept { return Outer; }

	GameObject* NewObject(SubclassOf<GameObject> InType, const String& InName = TEXT(""));

	template<std::derived_from<GameObject> T>
	T* NewObject(SubclassOf<T> InType, const String& InName = TEXT(""))
	{
		return Cast<T>(NewObject((SubclassOf<GameObject>)InType, InName));
	}

	template<std::derived_from<GameObject> T>
	T* NewObject(const String& InName = TEXT(""))
	{
		return NewObject<T>(T::StaticClass(), InName);
	}
};