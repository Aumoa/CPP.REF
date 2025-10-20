// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Scene.gen.h"

namespace Ayla
{
	class GameObject;

	ACLASS()
	class ENGINE_API Scene : public SerializableObject
	{
		GENERATED_BODY()

	private:
		std::vector<SharedPtr<GameObject>> m_GameObjects;
		bool m_Active = false;
		bool m_Disposed = false;

	public:
		ACONSTRUCTOR()
		Scene();
		virtual ~Scene() noexcept override;

		AFUNCTION()
		void Activate();
		AFUNCTION()
		void Destroy();

		AFUNCTION()
		SharedPtr<GameObject> SpawnGameObject();
	};
}