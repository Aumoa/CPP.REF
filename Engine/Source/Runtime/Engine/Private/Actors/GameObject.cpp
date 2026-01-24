// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Transform.h"

namespace Ayla
{
	GameObject::GameObject()
	{
		m_Transform = AddComponent<Transform>();
	}

	GameObject::~GameObject() noexcept
	{
	}

	SharedPtr<Transform> GameObject::GetTransform() const noexcept
	{
		return m_Transform;
	}
}