// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "EditorInitializationContext.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "HAL/PlatformRenderFeature.h"
#include "HAL/Graphics.h"
#include "HAL/Window.h"

namespace Ayla
{
	EditorEngine::EditorEngine()
	{
	}

	EditorEngine::~EditorEngine() noexcept
	{
	}

	RPtr<InitializationContext> EditorEngine::PreInitialize()
	{
		GenericSplash::Show();
		return New<EditorInitializationContext>();
	}

	void EditorEngine::Initialize(RPtr<InitializationContext> context, RPtr<PlatformRenderFeature> prf, RPtr<GenericApplication> app)
	{
		Super::Initialize(context, prf, app);

		GenericWindowDefinition wDef =
		{
			.bPrimaryWindow = true,
			.bSystemMenu = true,
			.bThickframe = true,
			.bSizebox = true,
			.bCaption = true
		};

		auto genericWindow = app->MakeWindow(wDef);
		m_EditorWindow = GetGraphics()->ConfigureWindow(genericWindow);

		GenericSplash::Hide();
		genericWindow->Show();
	}

	void EditorEngine::Tick()
	{
		Super::Tick();
		m_EditorWindow->OnPreRender();
		m_EditorWindow->OnGUI();
		m_EditorWindow->Present();
	}
}