// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "NamedObject.h"
#include "Actor.gen.h"

namespace Ayla
{
	class Component;

	ACLASS()
	class ENGINE_API Actor : public NamedObject
	{
		GENERATED_BODY()

	private:
		bool m_IsActive = false;
		std::vector<SharedPtr<Component>> m_Components;

	public:
		Actor();
		virtual ~Actor() noexcept override;

		AFUNCTION()
		void SetActive(bool active);
		AFUNCTION()
		bool IsActiveSelf() const { return m_IsActive; }
		AFUNCTION()
		SharedPtr<Component> AddComponent(ManagedTypeWrapper componentType);

		template<std::derived_from<Component> TComponent>
		SharedPtr<TComponent> AddComponent()
		{
			return AddComponent(TComponent::GetManagedType()).template As<TComponent>();
		}
	};
}