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

	void Engine::PreInitialize()
	{
	}

	void Engine::Initialize(const CommandLineParser* args)
	{
		bool initialized = false;
		try__
		{
			InitializeActivity();
			InitializeGraphics();
			InitializeGame(args);
			PostInitialized();
			initialized = true;
		}
		finally__
		{
			if (initialized == false)
			{
				m_RenderThread->Dispatch([]() {});
				m_RenderThread->Join();
			}
		}
		end_try__;
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
			auto gameAssembly = it->second[0].value();
			auto lib = DynamicLibrary(gameAssembly);
			if (lib.IsValid() == false)
			{
				throw ModuleNotFoundException(gameAssembly);
			}

			auto loader = lib.LoadFunction<RPtr<GameInstance>*>(TEXT("CreateGameInstance__"));
			if (loader == nullptr)
			{
				throw InvalidOperationException(String::Format(TEXT("Failed to load game instance from {0}. Ensure that the GameInstance class is exposed in the assembly via the DEFINE_GAME_INSTANCE_CLASS() macro."), gameAssembly));
			}

			auto rptrPtr = loader();
			m_GameInstance = *rptrPtr;
			delete rptrPtr;
			lib.Detach();
		}
		else
		{
			m_GameInstance = New<GameInstance>();
		}

		std::ignore = SceneManager::LoadSceneAsync(m_GameInstance->GetEntryScene());
	}

	void Engine::PostInitialized()
	{
		m_MainActivity->AfterInitialize();
	}
}