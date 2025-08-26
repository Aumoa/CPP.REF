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
		static RPtr<Scene> s_ActiveScene;
		static std::vector<RPtr<Scene>> s_AdditiveScenes;

	public:
		static Task<> LoadSceneAsync(RPtr<Scene> scene, LoadSceneMode mode = LoadSceneMode::Single, CancellationToken cancellationToken = CancellationToken::None());
	};
}