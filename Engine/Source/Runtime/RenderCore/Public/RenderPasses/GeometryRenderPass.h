// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderPasses/RenderPass.h"

namespace Ayla
{
	class RenderPipeline;
	class RenderTexture;

	class RENDERCORE_API GeometryRenderPass : public RenderPass
	{
		RenderPipeline* m_RenderPipeline;
		RenderTexture* m_RenderTexture;

	public:
		GeometryRenderPass(RenderPipeline* renderPipeline, RenderTexture* renderTexture);
		virtual ~GeometryRenderPass() noexcept override;

		virtual void Setup() override;
		virtual void Execute(CommandBuffer* commandBuffer, const SceneView& sceneView) override;
	};
}