// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Transform.h"

namespace Ayla
{
	GameObject::GameObject()
	{
		AddComponent<Transform>();
	}

	GameObject::~GameObject() noexcept
	{
	}
}