// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "Ticking/TickTiming.h"
#include "InitializeTiming.h"
#include "GameInstance.gen.h"

namespace Ayla
{
	class Scene;
	class SceneManager;
	class Engine;
	class MainSynchronizationContext;
	class TickManager;

	ACLASS()
	class ENGINE_API GameInstance : public SerializableObject
	{
		GENERATED_BODY()

	private:
		Engine* m_Engine = nullptr;
		std::shared_ptr<SceneManager> m_SceneManager;
		SharedPtr<Scene> m_EntryScene;
		std::shared_ptr<MainSynchronizationContext> m_SyncContext;
		std::unique_ptr<TickManager> m_TickManager;

	public:
		GameInstance();
		virtual ~GameInstance() noexcept override;

		virtual Task<> InitializeAsync(std::stop_token cancellationToken);
		void Tick(TickTiming timing, const TimeSpan& deltaTime);
		void Shutdown();

		AFUNCTION()
		SharedPtr<Engine> GetEngine();
		AFUNCTION()
		virtual SharedPtr<Scene> GetEntryScene();

		SceneManager* GetSceneManager() const noexcept { return m_SceneManager.get(); }
		TickManager* GetTickManager() const noexcept { return m_TickManager.get(); }

	protected:
		AFUNCTION()
		void InternalSetEngine(SharedPtr<Engine> engine);

		AFUNCTION()
		virtual void ScriptingInitialize(InitializeTiming timing);
	};
}