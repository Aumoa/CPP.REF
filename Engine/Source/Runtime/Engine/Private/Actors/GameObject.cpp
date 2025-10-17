// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Component.h"

namespace Ayla
{
	GameObject::GameObject()
	{
	}

	GameObject::~GameObject() noexcept
	{
	}

	void GameObject::SetActive(bool active)
	{
	}

	SharedPtr<Component> GameObject::AddComponent(ManagedTypeWrapper componentType)
	{
		throw InvalidOperationException(TEXT("There is no valid constructor."));
	}
}