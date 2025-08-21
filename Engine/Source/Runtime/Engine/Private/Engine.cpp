// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"
#include "GameInstance.h"
#include "CommandLineParser.h"
#include "Platform/DynamicLibrary.h"
#include "Rendering/RenderThread.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	Engine::~Engine() noexcept
	{
	}

	void Engine::PreInitialize()
	{
	}

	void Engine::Initialize(const CommandLineParser* args)
	{
		InitializeActivity();
		InitializeGraphics();
		InitializeGame(args);
		PostInitialized();
	}

	void Engine::Shutdown()
	{
		m_RenderThread->Join();
	}

	void Engine::Tick()
	{
		m_RenderThread->Dispatch([swapchainExtensions = m_SwapchainExtensions, graphics = m_Graphics]()
		{
			for (auto& swapchainExt : swapchainExtensions)
			{
				swapchainExt->Present();
			}
		});
	}

	void Engine::InitializeActivity()
	{
		m_MainActivity = GenericApplication::Get().CreateMainActivity();
		m_MainActivity->BeforeInitialize();
	}

	void Engine::InitializeGraphics()
	{
		m_Graphics = Graphics::CreateGraphics(RenderFeatures::Vulkan);
		m_SwapchainExtensions.emplace_back(m_Graphics->InstallSwapChain(m_MainActivity->GetMainWindow()));
		m_RenderThread = std::make_unique<RenderThread>(m_Graphics);
	}

	void Engine::InitializeGame(const CommandLineParser* args)
	{
		auto& options = args->Options();
		auto it = options.find(TEXT("gameassembly"));
		if (it != options.end())
		{
			auto gameAssembly = it->second[0];
			auto lib = DynamicLibrary(gameAssembly.value());
			auto loader = lib.LoadFunction<RPtr<GameInstance>*>(TEXT("CreateGameInstance__"));
			auto rptrPtr = loader();
			m_GameInstance = *rptrPtr;
			delete rptrPtr;
			lib.Detach();
		}
		else
		{
			m_GameInstance = New<GameInstance>();
		}
	}

	void Engine::PostInitialized()
	{
		m_MainActivity->AfterInitialize();
	}
}