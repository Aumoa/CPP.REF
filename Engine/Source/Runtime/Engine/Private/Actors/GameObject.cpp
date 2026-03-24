// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Transform.h"

namespace Ayla
{
	GameObject::GameObject(Scene* scene)
		: Super(scene)
	{
		m_Transform = AddComponent<Transform>().Get();
	}

	GameObject::~GameObject() noexcept
	{
	}
}