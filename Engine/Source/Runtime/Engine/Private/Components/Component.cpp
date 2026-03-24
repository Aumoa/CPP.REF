// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Components/Component.h"
#include "Actors/GameObject.h"

namespace Ayla
{
	Component::Component()
	{
	}

	Component::~Component() noexcept
	{
	}

	GameObject* Component::GetGameObject() const
	{
		auto actor = GetActor();
		return actor ? dynamic_cast<GameObject*>(actor) : nullptr;
	}

	Transform* Component::GetTransform() const
	{
		auto gameObject = GetGameObject();
		return gameObject ? gameObject->GetTransform() : nullptr;
	}
}