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

	SharedPtr<GameObject> Component::GetGameObject() const
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());
		return m_GameObjectPtr->AsShared();
	}
}