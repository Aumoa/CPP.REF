// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "Graphics.h"
#include "GenericApplication.h"
#include "GenericActivity.h"
#include "GenericWindowSwapchainExtension.h"

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

	void Engine::Initialize()
	{
		m_MainActivity = GenericApplication::Get().CreateMainActivity();
		m_MainActivity->BeforeInitialize();

		m_Graphics = Graphics::CreateGraphics(RenderFeatures::Vulkan);
		m_SwapchainExtensions.emplace_back(m_Graphics->InstallSwapChain(m_MainActivity->GetMainWindow()));

		m_MainActivity->AfterInitialize();
	}

	void Engine::Tick()
	{
		for (auto& swapchainExt : m_SwapchainExtensions)
		{
			swapchainExt->Present();
		}
	}
}