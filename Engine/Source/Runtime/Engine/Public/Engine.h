// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickTiming.h"
#include "Engine.gen.h"

namespace Ayla
{
	class Graphics;
	class GenericWindowSwapchainExtension;
	class RenderThread;
	class CommandBuffer;
	class TimerManager;
	class GenericActivity;
	class GameInstance;
	class Camera;
	class Shader;

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

		SharedPtr<Shader> m_DefaultRaygenShader;
		SharedPtr<Shader> m_DefaultClosestHitShader;
		SharedPtr<Shader> m_DefaultMissShader;

		struct Scratch
		{
			std::vector<Camera*> AllCameras;
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