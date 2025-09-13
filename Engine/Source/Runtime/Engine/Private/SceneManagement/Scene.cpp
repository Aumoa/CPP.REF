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
	}

	void Scene::Destroy()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, TEXT("Scene"));
		m_GameObjects.clear();
		m_Active = false;
		m_Disposed = true;
	}

	std::shared_ptr<GameObject> Scene::SpawnGameObject()
	{
		ObjectDisposedException::ThrowIfDisposed(m_Disposed, TEXT("Scene"));
		auto obj = New<GameObject>();
		m_GameObjects.emplace_back(obj);
		return obj;
	}
}