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

	SharedPtr<Actor> Component::GetActor() const
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());
		return m_ActorPtr->AsShared();
	}
}