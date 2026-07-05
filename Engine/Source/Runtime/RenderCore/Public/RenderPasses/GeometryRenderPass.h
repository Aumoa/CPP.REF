// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderPasses/RenderPass.h"

namespace Ayla
{
	class GraphicsRenderPipeline;
	class RenderTexture;
	class Buffer;

	class RENDERCORE_API GeometryRenderPass : public RenderPass
	{
		GraphicsRenderPipeline* m_RenderPipeline;
		RenderTexture* m_RenderTexture;
		Buffer* m_VertexBuffer;
		Buffer* m_IndexBuffer;

	public:
		GeometryRenderPass(GraphicsRenderPipeline* renderPipeline, RenderTexture* renderTexture, Buffer* vertexBuffer, Buffer* indexBuffer);
		virtual ~GeometryRenderPass() noexcept override;

		virtual void Setup() override;
		virtual void Execute(CommandBuffer* commandBuffer, const SceneView& sceneView) override;
	};
}
