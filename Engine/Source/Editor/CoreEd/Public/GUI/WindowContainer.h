// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Rect.h"
#include "WindowContainer.gen.h"

namespace Ayla
{
	class Window;
	class GenericApplication;
	class Engine;
	class GUI;
	struct GenericPlatformInputMouseMoveEvent;
	struct GenericPlatformInputMouseButtonEvent;

	ACLASS()
	class COREED_API WindowContainer : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<Window> m_GraphicsWindow;
		bool m_NeedRepaint = true;

	public:
		virtual void ConfigureWindow(RPtr<Engine> engine, RPtr<GenericApplication> app);

	public:
		virtual void DispatchMouseMove(const GenericPlatformInputMouseMoveEvent& event);
		virtual void DispatchMouseButton(const GenericPlatformInputMouseButtonEvent& event);
		virtual void RenderWindow();

	public:
		void Repaint();

	protected:
		virtual void OnGUI(const RectF& position);
	};
}