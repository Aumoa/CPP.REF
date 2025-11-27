// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	SceneRenderer::SceneRenderer(SharedPtr<RenderTexture> outputTexture)
		: m_OutputTexture(std::move(outputTexture))
	{
	}

	SceneRenderer::~SceneRenderer() noexcept
	{
	}
}