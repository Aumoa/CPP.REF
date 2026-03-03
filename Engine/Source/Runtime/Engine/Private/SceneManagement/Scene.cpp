// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "SceneManagement/Scene.h"
#include "Actors/GameObject.h"

namespace Ayla
{
	Scene::Scene()
	{
	}

	Scene::~Scene() noexcept
	{
	}

	void Scene::Activate()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, TEXT("Scene"));
		m_Active = true;
		for (auto& gameObject : m_InactiveGameObjects)
		{
			m_GameObjects.emplace_back(gameObject);
			gameObject->SetActive(true);
		}
		m_InactiveGameObjects.clear();
	}

	void Scene::Destroy()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, TEXT("Scene"));
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->Destroy();
		}
		m_GameObjects.clear();
		m_Active = false;
		m_Disposed = true;
	}

	SharedPtr<GameObject> Scene::SpawnGameObject()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, TEXT("Scene"));
		auto obj = New<GameObject>(this);
		if (m_Active)
		{
			m_GameObjects.emplace_back(obj);
			obj->SetActive(true);
		}
		else
		{
			m_InactiveGameObjects.emplace_back(obj);
		}
		return obj;
	}

	void Scene::AddCameraComponent(Camera* camera)
	{
		m_CameraComponents.emplace_back(camera);
	}

	void Scene::RemoveCameraComponent(Camera* camera)
	{
		auto it = std::find(m_CameraComponents.begin(), m_CameraComponents.end(), camera);
		check(it != m_CameraComponents.end());
		m_CameraComponents.erase(it);
	}

	void Scene::DispatchTick(TickTiming timing)
	{
		for (auto& gameObject : m_GameObjects)
		{
			gameObject->Tick(timing);
		}
	}
}