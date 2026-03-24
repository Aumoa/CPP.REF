// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "RenderPasses/GeometryRenderPass.h"
#include "CommandBuffer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderPipeline.h"

namespace Ayla
{
	GeometryRenderPass::GeometryRenderPass(RenderPipeline* renderPipeline, RenderTexture* renderTexture)
		: m_RenderPipeline(renderPipeline)
		, m_RenderTexture(renderTexture)
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
		commandBuffer->Draw();
		commandBuffer->EndRenderPass(m_RenderTexture);
	}
}