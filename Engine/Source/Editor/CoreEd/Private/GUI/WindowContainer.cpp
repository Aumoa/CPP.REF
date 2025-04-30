// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/WindowContainer.h"
#include "Engine.h"
#include "GenericPlatform/GenericApplication.h"
#include "HAL/Window.h"
#include "HAL/Graphics.h"
#include "HAL/IMGUI/GUI.h"

namespace Ayla
{
	void WindowContainer::ConfigureWindow(RPtr<Engine> engine, RPtr<GenericApplication> app)
	{
		GenericWindowDefinition wDef =
		{
			.bPrimaryWindow = true,
			.bSystemMenu = true,
			.bThickframe = true,
			.bSizebox = true,
			.bCaption = true
		};

		auto genericWindow = app->MakeWindow(wDef);
		m_GraphicsWindow = engine->ConfigureWindow(genericWindow);

		genericWindow->Show();
	}

	void WindowContainer::DrawGUI()
	{
		m_GraphicsWindow->BeginGUI();
		auto size = m_GraphicsWindow->GetSize();
		OnGUI(RectF(0, 0, size.X, size.Y));
		m_GraphicsWindow->EndGUI();
	}

	void WindowContainer::OnGUI(const RectF& position)
	{
		GUI::Label(position, TEXT("Window Container"));
	}
}