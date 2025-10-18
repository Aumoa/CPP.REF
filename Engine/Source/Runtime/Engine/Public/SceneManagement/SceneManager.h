// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneManagement/LoadSceneMode.h"
#include "SceneManager.gen.h"

namespace Ayla
{
	class Scene;

	ACLASS()
	class ENGINE_API SceneManager : public Object
	{
		GENERATED_BODY()

	private:
		SharedPtr<Scene> m_ActiveScene;
		std::vector<SharedPtr<Scene>> m_AdditiveScenes;

	public:
		ACONSTRUCTOR()
		SceneManager();
		virtual ~SceneManager() noexcept override;

		AFUNCTION()
		void LoadSceneAsync(SharedPtr<Scene> scene, LoadSceneMode mode = LoadSceneMode::Single, CancellationToken cancellationToken = CancellationToken::None());
	};
}