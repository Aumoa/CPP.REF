// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Engine.h"
#include "InitializationContext.h"
#include "HAL/PlatformRenderFeature.h"
#include "HAL/Graphics.h"
#include "GenericPlatform/GenericApplication.h"

namespace Ayla
{
	Engine::Engine()
	{
	}

	Engine::~Engine() noexcept
	{
	}

	RPtr<InitializationContext> Engine::PreInitialize()
	{
		return New<InitializationContext>();
	}

	void Engine::Initialize(RPtr<InitializationContext> context, RPtr<PlatformRenderFeature> prf, RPtr<GenericApplication> app)
	{
		m_App = app;

		context->Progress(TEXT("Initializing graphics..."));
		m_Graphics = prf->CreateGraphics(RenderAPI::D3D12);
		m_Graphics->Initialize();
	}

	void Engine::Tick(const std::vector<GenericPlatformInputEvent>& inputEvents)
	{
		m_Graphics->BeginRender();
		RenderWindows();
		m_Graphics->EndRender();
	}

	RPtr<Window> Engine::ConfigureWindow(RPtr<GenericWindow> targetWindow)
	{
		return m_Graphics->ConfigureWindow(targetWindow);
	}

	void Engine::RenderWindows()
	{
	}

	RPtr<Graphics> Engine::GetGraphics()
	{
		return m_Graphics;
	}
}