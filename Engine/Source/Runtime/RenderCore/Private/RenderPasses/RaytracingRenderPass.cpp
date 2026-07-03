// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "RenderPasses/RaytracingRenderPass.h"
#include "CommandBuffer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderPipeline.h"

namespace Ayla
{
	RaytracingRenderPass::RaytracingRenderPass(RenderPipeline* renderPipeline, RenderTexture* renderTexture)
		: m_RenderPipeline(renderPipeline)
		, m_RenderTexture(renderTexture)
	{
	}

	RaytracingRenderPass::~RaytracingRenderPass() noexcept
	{
	}

	void RaytracingRenderPass::Setup()
	{
	}

	void RaytracingRenderPass::Execute(CommandBuffer* commandBuffer, const SceneView& sceneView)
	{
		commandBuffer->SetRenderPipeline(m_RenderPipeline);
		m_RenderPipeline->SetCameraBufferView(commandBuffer, sceneView.CameraBuffer, sceneView.CameraBufferOffset);
		commandBuffer->DispatchRays(m_RenderTexture);
	}
}
