// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/RaytracingSceneRenderer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"

namespace Ayla
{
	RaytracingSceneRenderer::RaytracingSceneRenderer(SharedPtr<RenderTexture> outputTexture)
		: SceneRenderer(std::move(outputTexture))
	{
	}

	void RaytracingSceneRenderer::Render(const SceneView& view)
	{
	}
}