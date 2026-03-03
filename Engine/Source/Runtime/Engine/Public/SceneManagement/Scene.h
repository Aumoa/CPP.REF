// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "TickTiming.h"
#include "Scene.gen.h"

namespace Ayla
{
	class GameObject;
	class Camera;

	ACLASS()
	class ENGINE_API Scene : public SerializableObject
	{
		GENERATED_BODY()

	private:
		std::vector<SharedPtr<GameObject>> m_InactiveGameObjects;
		std::vector<SharedPtr<GameObject>> m_GameObjects;
		bool m_Active = false;
		bool m_Disposed = false;
		std::vector<Camera*> m_CameraComponents;

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

		std::span<Camera* const> GetCameraComponents() const { return m_CameraComponents; }
		void AddCameraComponent(Camera* camera);
		void RemoveCameraComponent(Camera* camera);

	public:
		void DispatchTick(TickTiming timing);
	};
}