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

		virtual void Tick();

	protected:
		RPtr<Graphics> GetGraphics() { return m_Graphics; }
	};
}