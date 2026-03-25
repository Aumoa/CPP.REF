// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Actors/GameObject.h"
#include "Components/Transform.h"
#include "Components/ScriptableBehavior.h"
#include "Activator.h"
#include "LogEngine.h"

namespace Ayla
{
	GameObject::GameObject(Scene* scene)
		: m_Scene(scene)
	{
		m_Transform = AddComponent<Transform>().Get();
	}

	GameObject::~GameObject() noexcept
	{
		check(m_Disposed);
	}

	void GameObject::SetActive(bool active)
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());

		if (m_IsActive == active)
		{
			return;
		}

		if (active)
		{
			for (auto& scriptableBehavior : m_ScriptableBehaviors)
			{
				if (scriptableBehavior->DidAwake() == false)
				{
					scriptableBehavior->Awake();
				}
			}

			for (auto& behavior : m_Behaviors)
			{
				behavior->OnEnable();
			}
		}
		else
		{
			for (auto& behavior : m_Behaviors)
			{
				behavior->OnDisable();
			}
		}
	}

	SharedPtr<Component> GameObject::AddComponent(ManagedTypeWrapper componentType)
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());

		auto obj = Activator::CreateInstance(componentType);
		SharedPtr<Component> comp;
		if (obj.Is(&comp) == false)
		{
			LogEngine::Error(TEXT("Failed to add component. The type is not a Component type."));
			return nullptr;
		}

		comp->m_GameObject = this;
		m_Components.emplace_back(comp);
		if (auto* scriptableBehavior = dynamic_cast<ScriptableBehavior*>(comp.Get()))
		{
			m_ScriptableBehaviors.emplace(scriptableBehavior);
			m_Behaviors.emplace(scriptableBehavior);

			if (m_IsActive)
			{
				scriptableBehavior->Awake();
				scriptableBehavior->OnEnable();
			}
		}
		else if (auto* behavior = dynamic_cast<Behavior*>(comp.Get()))
		{
			m_Behaviors.emplace(behavior);

			if (m_IsActive)
			{
				behavior->OnEnable();
			}
		}

		return comp;
	}

	void GameObject::Destroy()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, ToString());
		SetActive(false);

		for (auto& scriptableBehavior : m_ScriptableBehaviors)
		{
			scriptableBehavior->OnDestroy();
		}

		m_Components.clear();
		m_Behaviors.clear();
		m_ScriptableBehaviors.clear();
		m_Disposed = true;
	}
}