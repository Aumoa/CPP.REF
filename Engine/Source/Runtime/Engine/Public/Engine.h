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
		std::vector<SharedPtr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;

	public:
		ACONSTRUCTOR()
		Engine();
		virtual ~Engine() noexcept override;

		AFUNCTION()
		virtual void GuardedLoop();
		AFUNCTION()
		void Shutdown();

		virtual void Tick();

	protected:
		AFUNCTION()
		void SetupSwapchainExtensions(std::vector<SharedPtr<GenericWindowSwapchainExtension>> extensions);
	};
}