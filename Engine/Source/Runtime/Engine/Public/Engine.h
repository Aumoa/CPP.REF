// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/MinimalViewInfo.h"
#include "Engine.gen.h"

namespace Ayla
{
	enum class TickTiming;
	class Graphics;
	class GenericWindowSwapchainExtension;
	class RenderThread;
	class CommandBuffer;
	class TimerManager;
	class GenericActivity;
	class GameInstance;
	class Camera;
	class Shader;
	class GraphicsRenderPipeline;
	class RaytracingRenderPipeline;
	class Buffer;

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
		SharedPtr<GameInstance> m_GameInstance;
		std::unique_ptr<TimerManager> m_TimerManager;
		double m_FrameTime = 0;
		size_t m_FrameCount = 0;

		SharedPtr<GraphicsRenderPipeline> m_DefaultGeometryRenderPipeline;
		SharedPtr<RaytracingRenderPipeline> m_DefaultRaytracingRenderPipeline;

		struct Scratch
		{
			std::vector<Camera*> AllCameras;
			std::vector<MinimalViewInfo> AllCameraViews;
			SharedPtr<Buffer> CameraBuffers;
		} m_Scratch;

	public:
		ACONSTRUCTOR()
		Engine();
		virtual ~Engine() noexcept override;

		AFUNCTION()
		virtual void Initialize();
		AFUNCTION()
		virtual void GuardedLoop();
		AFUNCTION()
		void Shutdown();

		virtual void Tick();

		AFUNCTION()
		SharedPtr<Graphics> GetGraphics() const;

	protected:
		AFUNCTION()
		virtual SharedPtr<Graphics> InitializeGraphics();
		AFUNCTION()
		virtual SharedPtr<GameInstance> InitializeGameInstance();

	private:
		void DispatchTick(TickTiming timing);
	};
}
