// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SlateRenderer.h"
#include "Shaders/SlateShader/SlateShader.h"

SlateRenderer::SlateRenderer(SceneRenderTarget& InRenderTarget, SSlateShader* SlateShader, const SlateWindowElementList& Elements) : Super(InRenderTarget)
	, Shader(SlateShader)
	, Elements(Elements)
{
}

void SlateRenderer::OnPopulateCommandLists(IRHIDeviceContext* Context)
{
}