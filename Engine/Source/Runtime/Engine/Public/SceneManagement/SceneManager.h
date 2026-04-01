// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneManagement/LoadSceneMode.h"

namespace Ayla
{
	class GameInstance;
	class Scene;
	class Camera;
	class RenderScene;
	class Renderer;

	class ENGINE_API SceneManager : public NonCopyable
	{
		GameInstance* m_GameInstance;
		SharedPtr<Scene> m_ActiveScene;
		std::vector<SharedPtr<Scene>> m_AdditiveScenes;
		std::shared_ptr<RenderScene> m_RenderScene;
		std::set<Renderer*> m_DirtyRenderers;

	public:
		SceneManager(GameInstance* gameInstance);
		virtual ~SceneManager() noexcept;

		void LoadScene(SharedPtr<Scene> scene, LoadSceneMode mode = LoadSceneMode::Single);
		void UnloadAllScenes();
		
		void GetAllCameraComponents(std::vector<Camera*>* output) const;
		GameInstance* GetGameInstance() const noexcept { return m_GameInstance; }
		RenderScene* GetRenderScene() const noexcept { return m_RenderScene.get(); }
		
		void MarkRendererDirty(Renderer* renderer);
		void UnmarkRendererDirty(Renderer* renderer);
	};
}