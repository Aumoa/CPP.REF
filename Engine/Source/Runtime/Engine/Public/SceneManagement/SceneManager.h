// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickTiming.h"
#include "SceneManagement/LoadSceneMode.h"

namespace Ayla
{
	class Scene;

	class ENGINE_API SceneManager : public NonCopyable
	{
		SharedPtr<Scene> m_ActiveScene;
		std::vector<SharedPtr<Scene>> m_AdditiveScenes;

	public:
		SceneManager();
		virtual ~SceneManager() noexcept;

		void LoadScene(SharedPtr<Scene> scene, LoadSceneMode mode = LoadSceneMode::Single);

	public:
		void DispatchTick(TickTiming timing);
	};
}