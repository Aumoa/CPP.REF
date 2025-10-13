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

	std::shared_ptr<Component> GameObject::AddComponent(ManagedTypeWrapper componentType)
	{
		for (auto& ctor : componentType.NativeType->GetConstructors())
		{
			auto component = ctor->Invoke(std::span<std::any const>{});
			if (component.has_value())
			{
				auto componentPtr = std::static_pointer_cast<Component>(std::any_cast<std::shared_ptr<Object>>(component));
				m_Components.emplace_back(componentPtr);
				return componentPtr;
			}
		}

		throw InvalidOperationException(TEXT("There is no valid constructor."));
	}
}