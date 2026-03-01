// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SerializableObject.h"
#include "TickTiming.h"
#include "InitializeTiming.h"
#include "GameInstance.gen.h"

namespace Ayla
{
	class Scene;
	class SceneManager;
	class Engine;
	class MainSynchronizationContext;

	ACLASS()
	class ENGINE_API GameInstance : public SerializableObject
	{
		GENERATED_BODY()

	private:
		Engine* m_Engine = nullptr;
		std::shared_ptr<SceneManager> m_SceneManager;
		SharedPtr<Scene> m_EntryScene;
		std::shared_ptr<MainSynchronizationContext> m_SyncContext;

	public:
		GameInstance();
		virtual ~GameInstance() noexcept override;

		virtual Task<> InitializeAsync(std::stop_token cancellationToken);
		void Tick(TickTiming timing, const TimeSpan& deltaTime);

		AFUNCTION()
		SharedPtr<Engine> GetEngine();
		AFUNCTION()
		virtual SharedPtr<Scene> GetEntryScene();

	protected:
		AFUNCTION()
		void InternalSetEngine(SharedPtr<Engine> engine);

		AFUNCTION()
		virtual void ScriptingInitialize(InitializeTiming timing);
	};
}