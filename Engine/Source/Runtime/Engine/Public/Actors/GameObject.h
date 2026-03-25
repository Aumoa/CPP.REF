// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "NamedObject.h"
#include "GameObject.gen.h"

namespace Ayla
{
	class Component;
	class Behavior;
	class ScriptableBehavior;
	class Scene;
	class Transform;

	ACLASS()
	class ENGINE_API GameObject : public NamedObject
	{
		GENERATED_BODY()

	private:
		bool m_Disposed : 1 = false;
		bool m_IsActive : 1 = false;
		Scene* m_Scene = nullptr;
		Transform* m_Transform = nullptr;
		std::vector<SharedPtr<Component>> m_Components;
		std::set<Behavior*> m_Behaviors;
		std::set<ScriptableBehavior*> m_ScriptableBehaviors;

	public:
		GameObject(Scene* scene);
		virtual ~GameObject() noexcept override;

		AFUNCTION()
		void SetActive(bool active);
		AFUNCTION()
		bool IsActiveSelf() const { return m_IsActive; }
		AFUNCTION()
		SharedPtr<Component> AddComponent(ManagedTypeWrapper componentType);
		AFUNCTION()
		void Destroy();

		Scene* GetScene() const noexcept { return m_Scene; }
		Transform* GetTransform() const noexcept { return m_Transform; }

		template<std::derived_from<Component> TComponent>
		SharedPtr<TComponent> AddComponent()
		{
			return AddComponent(TComponent::GetManagedType()).template As<TComponent>();
		}
	};
}