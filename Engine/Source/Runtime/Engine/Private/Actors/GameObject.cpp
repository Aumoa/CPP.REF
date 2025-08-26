// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"

namespace Ayla
{
	GameObject::GameObject()
		: m_Active(true)
	{
	}

	GameObject::~GameObject() noexcept
	{
	}

	void GameObject::SetActive(bool active)
	{
	}

	bool GameObject::IsActiveSelf()
	{
		return m_Active;
	}
}