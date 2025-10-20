// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Component.h"
#include "Activator.h"
#include "LogEngine.h"

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
		auto obj = Activator::CreateInstance(componentType);
		SharedPtr<Component> comp;
		if (obj.Is(&comp) == false)
		{
			LogEngine::Error(TEXT("Failed to add component. The type is not a Component type."));
		}

		return comp;
	}
}