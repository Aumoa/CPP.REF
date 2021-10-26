// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SlateRenderer.h"
#include "Shaders/SlateShader/SlateShader.h"

SlateRenderer::SlateRenderer(SceneRenderTarget& InRenderTarget, SSlateShader* SlateShader, const SSlateShader::InitElementContext* InitContext) : Super(InRenderTarget)
	, Shader(SlateShader)
	, InitContext(InitContext)
{
}

void SlateRenderer::OnPopulateCommandLists(IRHIDeviceContext* Context)
{
	const Vector2 ViewportSz(RenderTarget.Viewport.Width, RenderTarget.Viewport.Height);
	Shader->RenderElements(Context, ViewportSz, *InitContext);
}