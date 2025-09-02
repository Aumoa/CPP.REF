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
		APROPERTY()
		bool m_IsActive;

	public:
		GameObject();
		virtual ~GameObject() noexcept override;

		void SetActive(bool active);
		bool IsActiveSelf() const { return m_IsActive; }
	};
}