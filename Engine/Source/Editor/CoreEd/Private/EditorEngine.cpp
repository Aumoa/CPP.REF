// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "EditorInitializationContext.h"
#include "EditorMainWindowContainer.h"
#include "GenericPlatform/GenericSplash.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/GenericApplication.h"
#include "HAL/PlatformRenderFeature.h"
#include "HAL/Graphics.h"

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
		m_EditorWindow = New<EditorMainWindowContainer>();
		m_EditorWindow->ConfigureWindow(rthis, app);
		GenericSplash::Hide();
	}

	void EditorEngine::Tick()
	{
		Super::Tick();
	}

	void EditorEngine::RenderWindows()
	{
		m_EditorWindow->DrawGUI();
	}
}