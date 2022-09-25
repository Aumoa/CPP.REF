// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "ObjectFlags.h"
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
	SPROPERTY()
	EObjectFlags ObjectFlags = EObjectFlags::None;

public:
	GameObject();

	void Rename(Object* InOuter, const String& InName);
	String GetName() noexcept { return Name; }
	Object* GetOuter() noexcept { return Outer; }

	void MarkObjectFlags(EObjectFlags InFlags) noexcept;
	void UnmarkObjectFlags(EObjectFlags InFlags) noexcept;
	void SetObjectFlags(EObjectFlags InFlags) noexcept;
	bool HasObjectFlags(EObjectFlags InFlags, bool bAny = false) noexcept;
	EObjectFlags GetObjectFlags() noexcept { return ObjectFlags; }

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