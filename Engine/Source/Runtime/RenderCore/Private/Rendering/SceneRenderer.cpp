// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderTexture.h"
#include "RenderPasses/RenderPass.h"

namespace Ayla
{
	SceneRenderer::SceneRenderer()
	{
	}

	SceneRenderer::~SceneRenderer() noexcept
	{
	}

	void SceneRenderer::AddPass(RenderPass* pass)
	{
		m_Passes.emplace_back(pass);
	}

	void SceneRenderer::Render(CommandBuffer* commandBuffer, const SceneView& view)
	{
		for (const auto& pass : m_Passes)
		{
			pass->Setup();
			pass->Execute(commandBuffer);
		}
	}
}