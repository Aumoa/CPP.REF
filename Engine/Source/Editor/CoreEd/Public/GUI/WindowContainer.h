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

	ACLASS()
	class COREED_API WindowContainer : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<Window> m_GraphicsWindow;

	public:
		virtual void ConfigureWindow(RPtr<Engine> engine, RPtr<GenericApplication> app);

	public:
		virtual void DrawGUI();

	protected:
		virtual void OnGUI(const RectF& position);
	};
}