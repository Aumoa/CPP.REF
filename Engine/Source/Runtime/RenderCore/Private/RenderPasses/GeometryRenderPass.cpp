// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "RenderPasses/GeometryRenderPass.h"
#include "CommandBuffer.h"

namespace Ayla
{
	GeometryRenderPass::GeometryRenderPass(RenderPipeline* renderPipeline)
		: m_RenderPipeline(renderPipeline)
	{
	}

	GeometryRenderPass::~GeometryRenderPass() noexcept
	{
	}

	void GeometryRenderPass::Setup()
	{
	}

	void GeometryRenderPass::Execute(CommandBuffer* commandBuffer)
	{
		commandBuffer->SetRenderPipeline(m_RenderPipeline);
	}
}