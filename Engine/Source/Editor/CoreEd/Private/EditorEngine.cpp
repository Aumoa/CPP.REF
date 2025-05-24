// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "EditorInitializationContext.h"
#include "GUI/EditorMainWindowContainer.h"
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

	void EditorEngine::HandleEventsForWindows(const std::vector<GenericPlatformInputEvent>& inputEvents)
	{
		if (inputEvents.size() > 0)
		{
			for (size_t i = 0; i < inputEvents.size(); ++i)
			{
				switch (inputEvents[i].Idx)
				{
				case GenericPlatformInputEvent::IDX_MouseMove:
					m_EditorWindow->DispatchMouseMove(inputEvents[i].MouseMove());
					break;
				case GenericPlatformInputEvent::IDX_MouseButton:
					m_EditorWindow->DispatchMouseMove(inputEvents[i].MouseMove());
					break;
				}
			}

			m_EditorWindow->Repaint();
		}
	}

	void EditorEngine::RenderWindows()
	{
		m_EditorWindow->RenderWindow();
	}
}