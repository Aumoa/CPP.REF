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
		std::unique_ptr<ScriptingBackend> m_ScriptingBackend;

		APROPERTY()
		PPtr<GameInstance> m_GameInstance;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual void PreInitialize();
		virtual void Initialize(const CommandLineParser* args);
		virtual void Shutdown();

		virtual void Tick();

	private:
		AFUNCTION()
		void InitializeActivity();
		AFUNCTION()
		void InitializeGraphics();
		void InitializeGame(const CommandLineParser* args);
		AFUNCTION()
		void PostInitialized();
	};
}