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

	RPtr<GameObject> Scene::SpawnGameObject()
	{
		auto obj = New<GameObject>();
		m_GameObjects.emplace_back(obj);
		return obj;
	}
}