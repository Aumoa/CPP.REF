// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderPasses/RenderPass.h"

namespace Ayla
{
	class RenderPipeline;
	class RenderTexture;

	class RENDERCORE_API RaytracingRenderPass : public RenderPass
	{
		RenderPipeline* m_RenderPipeline;
		RenderTexture* m_RenderTexture;

	public:
		RaytracingRenderPass(RenderPipeline* renderPipeline, RenderTexture* renderTexture);
		virtual ~RaytracingRenderPass() noexcept override;

		virtual void Setup() override;
		virtual void Execute(CommandBuffer* commandBuffer, const SceneView& sceneView) override;
	};
}
