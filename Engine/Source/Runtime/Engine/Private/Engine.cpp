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
		MainSynchronizationContext syncContext;
		SynchronizationContext::SetSynchronizationContext(&syncContext);

		auto& app = GenericApplication::Get();
		std::vector<GenericPlatformInputEvent> inputEvents;
		m_TimerManager = std::make_unique<TimerManager>();
		m_TimerManager->Start();

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

		for (auto& swapchain : m_SwapchainExtensions)
		{
			swapchain->Destroy();
		}

		m_SwapchainExtensions.clear();
	}

	void Engine::Tick()
	{
		m_TimerManager->StartFrame();
		MainSynchronizationContext::GetCurrent()->Tick();

		m_RenderThread->Dispatch([
			swapchainExtensions = m_SwapchainExtensions,
			graphics = m_Graphics,
			commandBuffer = m_CommandBuffer
		]()
		{
			graphics->BeginRenderFrame();

			// SceneView: Overlay, #0
			auto rt = swapchainExtensions[0]->GetRenderTexture();
			SceneView view(rt);
			RaytracingSceneRenderer renderer;

			commandBuffer->BeginCommands();

			rt->Acquire(commandBuffer.Get());

			commandBuffer->EndCommands();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Present(commandBuffer.Get());
			}

			graphics->EndRenderFrame();
		});

		auto fps = 1.0 / m_TimerManager->GetDeltaTime().GetTotalSeconds();
		m_MainActivity->SetTitle(String::Format(TEXT("FPS: {:.2f}"), fps));
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