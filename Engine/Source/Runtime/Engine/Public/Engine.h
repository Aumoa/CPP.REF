// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class InitializationContext;
	class PlatformRenderFeature;
	class Graphics;
	class GenericApplication;
	class Window;
	class GenericWindow;
	struct GenericPlatformInputEvent;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		APROPERTY()
		PPtr<GenericApplication> m_App;
		APROPERTY()
		PPtr<Graphics> m_Graphics;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual RPtr<InitializationContext> PreInitialize();
		virtual void Initialize(RPtr<InitializationContext> context, RPtr<PlatformRenderFeature> prf, RPtr<GenericApplication> app);

		virtual void Tick(const std::vector<GenericPlatformInputEvent>& inputEvents);

	public:
		RPtr<Window> ConfigureWindow(RPtr<GenericWindow> targetWindow);

	protected:
		virtual void RenderWindows();

	protected:
		RPtr<Graphics> GetGraphics();
	};
}