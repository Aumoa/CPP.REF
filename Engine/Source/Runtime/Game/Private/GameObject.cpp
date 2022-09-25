// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameObject.h"
#include "Reflection/ConstructorInfo.h"
#include "GameObject.gen.cpp"

GameObject::GameObject() : Super()
{
}

void GameObject::Rename(Object* InOuter, const String& InName)
{
	Outer = InOuter;
	Name = InName;
}

void GameObject::MarkObjectFlags(EObjectFlags InFlags) noexcept
{
	ObjectFlags |= InFlags;
}

void GameObject::UnmarkObjectFlags(EObjectFlags InFlags) noexcept
{
	ObjectFlags &= ~InFlags;
}

void GameObject::SetObjectFlags(EObjectFlags InFlags) noexcept
{
	ObjectFlags = InFlags;
}

bool GameObject::HasObjectFlags(EObjectFlags InFlags, bool bAny) noexcept
{
	if (bAny)
	{
		using T = std::underlying_type_t<EObjectFlags>;
		return (T)(ObjectFlags & InFlags) > 0;
	}
	else
	{
		return (ObjectFlags & InFlags) == InFlags;
	}
}

GameObject* GameObject::NewObject(SubclassOf<GameObject> InType, const String& InName)
{
	check(InType.IsValid());

	ConstructorInfo* Constructor = InType->GetConstructors()[0];
	check(Constructor);

	GameObject* Object = Cast<GameObject>(Constructor->Invoke());

	if (InName == TEXT(""))
	{
		Object->Rename(this, InType->GetName());
	}
	else
	{
		Object->Rename(this, InName);
	}

	return Object;
}