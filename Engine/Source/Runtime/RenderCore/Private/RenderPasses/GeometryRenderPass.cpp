// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "RenderPasses/GeometryRenderPass.h"
#include "CommandBuffer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderPipeline.h"

namespace Ayla
{
	GeometryRenderPass::GeometryRenderPass(GraphicsRenderPipeline* renderPipeline, RenderTexture* renderTexture, Buffer* vertexBuffer, Buffer* indexBuffer)
		: m_RenderPipeline(renderPipeline)
		, m_RenderTexture(renderTexture)
		, m_VertexBuffer(vertexBuffer)
		, m_IndexBuffer(indexBuffer)
	{
	}

	GeometryRenderPass::~GeometryRenderPass() noexcept
	{
	}

	void GeometryRenderPass::Setup()
	{
	}

	void GeometryRenderPass::Execute(CommandBuffer* commandBuffer, const SceneView& sceneView)
	{
		commandBuffer->BeginRenderPass(m_RenderTexture);
		commandBuffer->SetRenderPipeline(m_RenderPipeline);
		m_RenderPipeline->SetCameraBufferView(commandBuffer, sceneView.CameraBuffer, sceneView.CameraBufferOffset);
		commandBuffer->Draw(m_VertexBuffer, m_IndexBuffer);
		commandBuffer->EndRenderPass(m_RenderTexture);
	}
}
