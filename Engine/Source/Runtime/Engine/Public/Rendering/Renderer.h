// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Behavior.h"
#include "Renderer.gen.h"

namespace Ayla
{
	class RendererProxy;

	ACLASS()
	class ENGINE_API Renderer : public Behavior
	{
		GENERATED_BODY()

	private:
		bool m_RenderStateDirty : 1 = false;
		std::shared_ptr<RendererProxy> m_Proxy;

	public:
		Renderer();
		virtual ~Renderer() noexcept override;

		std::shared_ptr<RendererProxy> UpdateAndGetProxy();

		virtual void OnEnable_Implementation() override;
		virtual void OnDisable_Implementation() override;

		void MarkRenderStateDirty();

	protected:
		virtual std::shared_ptr<RendererProxy> CreateProxy();
	};
}