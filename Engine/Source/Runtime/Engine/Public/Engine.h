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
	class GraphicsWindow;
	class GenericWindow;
	struct GenericPlatformInputEvent;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		std::shared_ptr<GenericApplication> m_App;
		APROPERTY()
		PPtr<Graphics> m_Graphics;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual RPtr<InitializationContext> PreInitialize();
		virtual void Initialize(RPtr<InitializationContext> context, RPtr<PlatformRenderFeature> prf, std::shared_ptr<GenericApplication> app);

		virtual void Tick(const std::vector<GenericPlatformInputEvent>& inputEvents);

	public:
		RPtr<Graphics> GetGraphics();

	protected:
		virtual void HandleEventsForWindows(const std::vector<GenericPlatformInputEvent>& inputEvents);
		virtual void RenderWindows();
	};
}