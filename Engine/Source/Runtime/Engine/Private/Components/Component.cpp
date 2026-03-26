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

	Transform* Component::GetTransform() const
	{
		return GetGameObject()->GetTransform();
	}

	Scene* Component::GetScene() const
	{
		return GetGameObject()->GetScene();
	}
}