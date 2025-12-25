// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/RaytracingSceneRenderer.h"
#include "Rendering/SceneView.h"
#include "Rendering/RenderTexture.h"
#include "CommandBuffer.h"

namespace Ayla
{
	RaytracingSceneRenderer::RaytracingSceneRenderer(SharedPtr<RenderTexture> outputTexture)
		: SceneRenderer(std::move(outputTexture))
	{
	}

	void RaytracingSceneRenderer::Render(const SceneView& view)
	{
		// Delegate to the backend-specific RenderTexture implementation
		// This allows each backend (Vulkan, D3D12) to implement raytracing in their own way
		GetOutputTexture()->RenderRaytracing(nullptr, view);
	}
}