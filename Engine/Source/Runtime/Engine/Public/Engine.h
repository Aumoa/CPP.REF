// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class Graphics;
	class GenericActivity;
	class GenericWindowSwapchainExtension;
	class RenderThread;
	class GameInstance;
	class CommandLineParser;
	class ScriptingBackend;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<GenericActivity> m_MainActivity;
		std::shared_ptr<Graphics> m_Graphics;
		std::vector<std::shared_ptr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;
		std::unique_ptr<RenderThread> m_RenderThread;

		APROPERTY()
		std::shared_ptr<GameInstance> m_GameInstance;

	public:
		ACONSTRUCTOR()
		Engine();
		virtual ~Engine() noexcept override;

		AFUNCTION()
		virtual void GuardedStartup();
		AFUNCTION()
		virtual void GuardedLoop();

		AFUNCTION()
		virtual void PreInitialize();
		AFUNCTION()
		virtual void Initialize();
		AFUNCTION()
		virtual void Shutdown();

		virtual void Tick();

	protected:
		AFUNCTION()
		void InitializeActivity();
		AFUNCTION()
		void InitializeGraphics();
		AFUNCTION()
		void PostInitialized();
	};
}