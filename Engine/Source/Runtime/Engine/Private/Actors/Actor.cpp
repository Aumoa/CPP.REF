// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"
#include "Components/Component.h"
#include "Activator.h"
#include "LogEngine.h"

namespace Ayla
{
	Actor::Actor()
	{
	}

	Actor::~Actor() noexcept
	{
	}

	void Actor::SetActive(bool active)
	{
	}

	SharedPtr<Component> Actor::AddComponent(ManagedTypeWrapper componentType)
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