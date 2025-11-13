// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"
#include "GameInstance.h"
#include "CommandLineParser.h"
#include "CommandBuffer.h"
#include "Platform/DynamicLibrary.h"
#include "Rendering/RenderThread.h"
#include "Exceptions/ModuleNotFoundException.h"
#include "SceneManagement/SceneManager.h"

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
		auto& app = GenericApplication::Get();
		std::vector<GenericPlatformInputEvent> inputEvents;
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
		for (auto& swapchain : m_SwapchainExtensions)
		{
			swapchain->Destroy();
		}

		m_SwapchainExtensions.clear();
	}

	void Engine::Tick()
	{
		m_RenderThread->Dispatch([
			swapchainExtensions = m_SwapchainExtensions,
			graphics = m_Graphics,
			commandBuffer = m_CommandBuffer
		]()
		{
			graphics->BeginRenderFrame();

			commandBuffer->BeginCommands();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Acquire(commandBuffer.Get());
			}

			commandBuffer->EndCommands();

			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Present(commandBuffer.Get());
			}

			graphics->EndRenderFrame();
		});
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