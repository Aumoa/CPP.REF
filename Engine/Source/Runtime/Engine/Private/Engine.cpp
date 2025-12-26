// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"
#include "GameInstance.h"
#include "CommandBuffer.h"
#include "TimerManager.h"
#include "Rendering/RenderThread.h"
#include "Exceptions/ModuleNotFoundException.h"
#include "SceneManagement/SceneManager.h"
#include "Rendering/RaytracingSceneRenderer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"
#include "Threading/MainSynchronizationContext.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	Engine::~Engine() noexcept
	{
	}

	void Engine::GuardedLoop_Implementation()
	{
		auto syncContext = std::make_shared<MainSynchronizationContext>();
		SynchronizationContext::SetSynchronizationContext(syncContext);

		auto& app = GenericApplication::Get();
		std::vector<GenericPlatformInputEvent> inputEvents;
		m_TimerManager = std::make_unique<TimerManager>();
		m_TimerManager->Start();
		
		m_TimerManager->AddInterval([this]()
		{
			auto dt = m_FrameTime / m_FrameCount;
			auto fps = 1.0 / dt;
			m_MainActivity->SetTitle(String::Format(TEXT("{}, FPS: {:.2f}"), m_Graphics->GetCurrentRenderFeature(), fps));
			m_FrameTime = 0;
			m_FrameCount = 0;
		}, TimeSpan::FromSeconds(1));

		while (true)
		{
			app.PumpMessages(inputEvents);
			if (app.IsQuitRequested())
			{
				break;
			}

			Tick();
		}
	}

	void Engine::Shutdown()
	{
		m_RenderThread->RequestStop();
		m_Graphics->WaitForCompletion();

		for (auto& swapchain : m_SwapchainExtensions)
		{
			swapchain->Destroy();
		}

		m_SwapchainExtensions.clear();

		m_CommandBuffer->Dispose();
		m_Graphics->Dispose();
	}

	void Engine::Tick()
	{
		m_TimerManager->StartFrame();

		MainSynchronizationContext::GetCurrent()->Tick();

		m_FrameTime += m_TimerManager->GetDeltaTime().GetTotalSeconds();
		m_FrameCount += 1;
		m_TimerManager->UpdateTasks();

		static constexpr MinimalViewInfo kSampleView =
		{
			.Position = Vector3D(0, 0, -10),
			.Rotation = QuaternionD::Identity(),
			.FieldOfView = 60.0,
			.AspectRatio = std::nullopt
		};

		m_RenderThread->Dispatch([
			swapchainExtensions = m_SwapchainExtensions,
			graphics = m_Graphics,
			commandBuffer = m_CommandBuffer,
			self = m_RenderThread.Get()
		]()
		{
			commandBuffer->WaitForCompletion(TimeSpan::FromSeconds(1));
			graphics->BeginRenderFrame();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->DoResize();
			}

			self->ExecuteJobs();

			commandBuffer->BeginCommands();

			// Camera: Overlay, Display #0
			auto rt = swapchainExtensions[0]->GetRenderTexture();
			rt->Acquire(commandBuffer.Get());

			SceneView view(kSampleView);
			RaytracingSceneRenderer renderer(rt);
			renderer.Render(view);

			commandBuffer->EndCommands();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Present(commandBuffer.Get());
			}

			graphics->EndRenderFrame();
		});
	}

	SharedPtr<Graphics> Engine::GetGraphics()
	{
		return m_Graphics;
	}

	void Engine::InitializeMainActivity(SharedPtr<GenericActivity> mainActivity)
	{
		m_MainActivity = mainActivity;
	}

	void Engine::InitializeGraphics(SharedPtr<Graphics> graphics)
	{
		m_Graphics = graphics;
		m_RenderThread = New<RenderThread>(graphics);
		m_CommandBuffer = graphics->CreateCommandBuffer();
	}

	void Engine::SetupSwapchainExtensions(std::vector<SharedPtr<GenericWindowSwapchainExtension>> extensions)
	{
		m_SwapchainExtensions = std::move(extensions);
	}
}