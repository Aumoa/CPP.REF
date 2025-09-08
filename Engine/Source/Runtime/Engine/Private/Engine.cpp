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
		try
		{
			InitializeActivity();
			InitializeGraphics();
			InitializeGame(args);
			PostInitialized();
		}
		catch (...)
		{
			Shutdown();
			throw;
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
		m_Graphics.reset();
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
			m_ScriptingBackend = std::make_unique<CoreCLRScriptingBackend>();
			m_ScriptingBackend->LoadAssembly(Path::GetDirectoryName(gameAssembly), Path::GetFileNameWithoutExtension(gameAssembly));

			using InitializeGame_ManagedFunc = void(*)(const wchar_t*);
			auto scriptInitialize = (InitializeGame_ManagedFunc)m_ScriptingBackend->GetFunctionPointer(TEXT("Engine.Script"), TEXT("Ayla.Engine"), TEXT("InitializeGame_Managed"));
			scriptInitialize(gameAssembly.c_str());
		}

		m_GameInstance = New<GameInstance>();
		std::ignore = SceneManager::LoadSceneAsync(m_GameInstance->GetEntryScene());
	}

	void Engine::PostInitialized()
	{
		m_MainActivity->AfterInitialize();
	}
}