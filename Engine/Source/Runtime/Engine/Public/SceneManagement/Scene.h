// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Scene.gen.h"

namespace Ayla
{
	class GameObject;
	class Camera;
	class SceneManager;

	ACLASS()
	class ENGINE_API Scene : public SerializableObject
	{
		GENERATED_BODY()

	private:
		std::vector<SharedPtr<GameObject>> m_InactiveGameObjects;
		std::vector<SharedPtr<GameObject>> m_GameObjects;
		bool m_Active : 1 = false;
		bool m_Disposed : 1 = false;
		std::vector<Camera*> m_CameraComponents;
		SceneManager* m_SceneManager = nullptr;

	public:
		ACONSTRUCTOR()
		Scene();
		virtual ~Scene() noexcept override;

		void Activate(SceneManager* sceneManager);
		void Destroy();

		AFUNCTION()
		SharedPtr<GameObject> SpawnGameObject();

		std::span<Camera* const> GetCameraComponents() const { return m_CameraComponents; }
		void AddCameraComponent(Camera* camera);
		void RemoveCameraComponent(Camera* camera);
		SceneManager* GetSceneManager() const noexcept { return m_SceneManager; }
	};
}