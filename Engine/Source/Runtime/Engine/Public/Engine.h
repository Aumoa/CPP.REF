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
	class TimerManager;
	class GenericActivity;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		SharedPtr<GenericActivity> m_MainActivity;
		SharedPtr<Graphics> m_Graphics;
		SharedPtr<RenderThread> m_RenderThread;
		std::vector<SharedPtr<GenericWindowSwapchainExtension>> m_SwapchainExtensions;
		SharedPtr<CommandBuffer> m_CommandBuffer;
		std::unique_ptr<TimerManager> m_TimerManager;
		double m_FrameTime = 0;
		size_t m_FrameCount = 0;

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
		void InitializeMainActivity(SharedPtr<GenericActivity> mainActivity);
		AFUNCTION()
		void InitializeGraphics(SharedPtr<Graphics> graphics);
		AFUNCTION()
		void SetupSwapchainExtensions(std::vector<SharedPtr<GenericWindowSwapchainExtension>> extensions);
	};
}