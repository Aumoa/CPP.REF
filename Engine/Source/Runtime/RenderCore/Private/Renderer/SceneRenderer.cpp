// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Renderer/SceneRenderer.h"
#include "RHI/RHIViewport.h"

NSceneRenderer::NSceneRenderer()
{
}

NSceneRenderer::~NSceneRenderer() noexcept
{
}

void NSceneRenderer::SetViewport(std::shared_ptr<NRHIViewport> InViewport)
{
	Viewport = InViewport;
}

std::shared_ptr<NRHIViewport> NSceneRenderer::GetViewport() const
{
	return Viewport;
}