// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/Actor.h"
#include "Components/Component.h"
#include "Components/Behavior.h"
#include "Components/Transform.h"
#include "Activator.h"
#include "LogEngine.h"

namespace Ayla
{
	Actor::Actor()
	{
	}

	Actor::~Actor() noexcept
	{
		check(m_Disposed);
	}

	void Actor::SetActive(bool active)
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());

		if (m_IsActive == active)
		{
			return;
		}

		if (active)
		{
			for (auto& component : m_Components)
			{
				if (Behavior* behavior; component.Is(&behavior))
				{
					behavior->OnEnable();
				}
			}
		}
		else
		{
			for (auto& component : m_Components)
			{
				if (Behavior* behavior; component.Is(&behavior))
				{
					behavior->OnDisable();
				}
			}
		}
	}

	SharedPtr<Component> Actor::AddComponent(ManagedTypeWrapper componentType)
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());

		auto obj = Activator::CreateInstance(componentType);
		SharedPtr<Component> comp;
		if (obj.Is(&comp) == false)
		{
			LogEngine::Error(TEXT("Failed to add component. The type is not a Component type."));
			return nullptr;
		}

		if (m_IsActive)
		{
			if (auto* behavior = (Behavior*)obj.Get(); behavior)
			{
				behavior->OnEnable();
			}
		}

		comp->m_ActorPtr = this;
		m_Components.emplace_back(comp);
		return comp;
	}

	void Actor::Destroy()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());
		SetActive(false);
		m_Components.clear();
		m_Disposed = true;
	}
}