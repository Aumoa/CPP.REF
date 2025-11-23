// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"
#include "GameInstance.h"
#include "CommandBuffer.h"
#include "Rendering/RenderThread.h"
#include "Exceptions/ModuleNotFoundException.h"
#include "SceneManagement/SceneManager.h"
#include "Rendering/RaytracingSceneRenderer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	Engine::~Engine() noexcept
	{
	}

	class MainSynchronizationContext : public SynchronizationContext
	{
	public:
		static std::mutex s_Mutex;
		static std::queue<SharedTask<>::function_t<void()>> s_Continuations;

	public:
		virtual void Post(SharedTask<>::function_t<void()> callback) override
		{
			std::unique_lock lock(s_Mutex);
			s_Continuations.emplace(std::move(callback));
		}
	};

	std::mutex MainSynchronizationContext::s_Mutex;
	std::queue<SharedTask<>::function_t<void()>> MainSynchronizationContext::s_Continuations;

	void Engine::GuardedLoop_Implementation()
	{
		MainSynchronizationContext syncContext;
		SynchronizationContext::SetSynchronizationContext(&syncContext);

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
		m_RenderThread->RequestStop();

		for (auto& swapchain : m_SwapchainExtensions)
		{
			swapchain->Destroy();
		}

		m_SwapchainExtensions.clear();
	}

	void Engine::Tick()
	{
		std::unique_lock lock(MainSynchronizationContext::s_Mutex);
		auto cc = std::move(MainSynchronizationContext::s_Continuations);
		lock.unlock();

		while (!cc.empty())
		{
			cc.front()();
			cc.pop();
		}

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