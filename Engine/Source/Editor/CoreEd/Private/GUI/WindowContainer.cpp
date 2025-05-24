// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "GUI/WindowContainer.h"
#include "Engine.h"
#include "GenericPlatform/GenericApplication.h"
#include "HAL/Window.h"
#include "HAL/Graphics.h"
#include "HAL/IMGUI/GUI.h"
#include "HAL/IMGUI/GUIEvent.h"

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

	void WindowContainer::DispatchMouseMove(const GenericPlatformInputMouseMoveEvent& event)
	{
		m_GraphicsWindow->BeginNonRenderGUI();
		GUIEvent e = GUIEvent::MouseMove(event.Location);
		GUI::SetEvent(&e);
		OnGUI(RectF(Vector2F::Zero(), Vector<>::Cast<Vector2F>(m_GraphicsWindow->GetSize())));
		GUI::SetEvent(nullptr);
		m_GraphicsWindow->EndNonRenderGUI();
	}

	void WindowContainer::DispatchMouseButton(const GenericPlatformInputMouseButtonEvent& event)
	{
		m_GraphicsWindow->BeginNonRenderGUI();
		GUIEvent e = (event.bUp ? GUIEvent::MouseUp : GUIEvent::MouseDown)(event.Location, (int32)event.ButtonType);
		GUI::SetEvent(&e);
		OnGUI(RectF(Vector2F::Zero(), Vector<>::Cast<Vector2F>(m_GraphicsWindow->GetSize())));
		GUI::SetEvent(nullptr);
		m_GraphicsWindow->EndNonRenderGUI();
	}

	void WindowContainer::RenderWindow()
	{
		if (m_NeedRepaint)
		{
			m_NeedRepaint = false;

			m_GraphicsWindow->BeginRenderGUI();
			GUIEvent e = GUIEvent::Repaint();
			GUI::SetEvent(&e);
			OnGUI(RectF(Vector2F::Zero(), Vector<>::Cast<Vector2F>(m_GraphicsWindow->GetSize())));
			GUI::SetEvent(nullptr);
			m_GraphicsWindow->EndRenderGUI();
		}
	}

	void WindowContainer::Repaint()
	{
		m_NeedRepaint = true;
	}

	void WindowContainer::OnGUI(const RectF& position)
	{
		GUI::Label(position, TEXT("Window Container"));
	}
}