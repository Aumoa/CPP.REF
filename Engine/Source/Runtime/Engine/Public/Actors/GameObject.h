// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "NamedObject.h"
#include "GameObject.gen.h"

namespace Ayla
{
	class Component;

	ACLASS()
	class ENGINE_API GameObject : public NamedObject
	{
		GENERATED_BODY()

	private:
		bool m_IsActive = false;
		std::vector<std::shared_ptr<Component>> m_Components;

	public:
		GameObject();
		virtual ~GameObject() noexcept override;

		AFUNCTION()
		void SetActive(bool active);
		AFUNCTION()
		bool IsActiveSelf() const { return m_IsActive; }
		std::shared_ptr<Component> AddComponent(Type* type);
	};
}