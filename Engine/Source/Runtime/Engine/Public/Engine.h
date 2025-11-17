// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class Graphics;
	class GenericWindowSwapchainExtension;
	class RenderThread;
	class CommandBuffer;
	class SceneRenderer;
	class SceneView;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		SharedPtr<Graphics> m_Graphics;
		SharedPtr<RenderThread> m_RenderThread;
		std::vector<SharedPtr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;
		SharedPtr<CommandBuffer> m_CommandBuffer;
		std::unique_ptr<SceneRenderer> m_SceneRenderer;

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
		void InitializeGraphics(SharedPtr<Graphics> graphics);
		AFUNCTION()
		void SetupSwapchainExtensions(std::vector<SharedPtr<GenericWindowSwapchainExtension>> extensions);
	};
}