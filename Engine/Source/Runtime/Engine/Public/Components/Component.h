// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Component.gen.h"

namespace Ayla
{
	class GameObject;

	ACLASS()
	class ENGINE_API Component : public SerializableObject
	{
		GENERATED_BODY()

	private:
		bool m_Disposed = false;
		GameObject* m_GameObjectPtr = nullptr;

	public:
		Component();
		virtual ~Component() noexcept override;

		AFUNCTION()
		SharedPtr<GameObject> GetGameObject() const;
	};
}