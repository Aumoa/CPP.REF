// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.gen.h"

namespace Ayla
{
	class InitializationContext;
	class PlatformRenderFeature;
	class Graphics;

	ACLASS()
	class ENGINE_API Engine : public Object
	{
		GENERATED_BODY()

	private:
		RPtr<Graphics> m_Graphics;

	public:
		Engine();
		virtual ~Engine() noexcept override;

		virtual RPtr<InitializationContext> PreInitialize();
		virtual void Initialize(RPtr<PlatformRenderFeature> prf, RPtr<InitializationContext> context);
	};
}